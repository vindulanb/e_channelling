#include	"utils.h"		/* Header file */
#include <stdio.h>
#include <stdlib.h>

//The considerable file -> str_echo.c
//[Refered file] This file is referd and mainly based on Practical 5 of DCCN3 module in (SLIIT)Srilanka institute of information technology.

//[Refered link]https://www.geeksforgeeks.org/bool-in-c/
typedef enum boolean {false, true} bool;

void str_echo(int sockfd)
{
	int selected_time_slot = 1;
	int comma_count = 0;
	int line_no = 0;
	int count;
	int	time_c;

	char selected_doc_Id[3];
	char selected_spec[10];
	char doc_id_c[3];
	char confirmation[3];
	char confirmation_a[3];

	ssize_t	n;
	char line[MAXLINE];

	bool is_available = false;
	bool is_atleast_one = false;

	char doc_list[300];
	char buff_temp[255];
	char res[256];
	char res_out[1000];
	char buff[1000];
	char buffer[255];
	char text_line[255];
	char newline[255];

	char *ret;

	FILE * fPtr;
	FILE * fTemp;
	FILE *fp;
	FILE *fp_spec;

	FILE *fp3 = fopen("welcome_message.txt", "r");
	if(fp3==NULL){
		snprintf(res_out, sizeof(res_out),"cant open file");
		return 1;
	}
	while(!feof(fp3)){
		fgets(res, sizeof(res), fp3);

		//To determine the end of the file in the middle of the loop
		//otherwise it may print last line twice
		//[Refered linke]https://stackoverflow.com/questions/29143338/c-program-prints-the-last-line-twice-file-i-o
		if (feof(fp3))
			break;

		snprintf(line, sizeof(line), res);
		n = strlen(line);
		Writen(sockfd, line, n);
	}
	fclose(fp3);

	for ( ; ; ) {

		if ((n = Readline(sockfd, line, MAXLINE)) == 0)
			return;		/* connection closed by other end */

		//https://regex101.com/
		//Checking for specialization code format.
		if (sscanf(line, "%s", &selected_spec) == 1)
		{
			//Checking for specialization length entered
			if (strlen(selected_spec) == 4)
			{
				fp_spec = fopen("database.txt", "r");
				//Display table title fields -> Id and Name
				snprintf(line, sizeof(line), "Id Name\n");
				n = strlen(line);
				Writen(sockfd, line, n);

				while(!feof(fp_spec))
				{
					fgets(doc_list,300,fp_spec);

					//To determine the end of the file in the middle of the loop
					//otherwise it may print last line twice
					//[Refered linke]https://stackoverflow.com/questions/29143338/c-program-prints-the-last-line-twice-file-i-o
					if (feof(fp_spec))
						break;

					 //[Refered linke]https://www.tutorialspoint.com/c_standard_library/c_function_strstr.htm
					 //function finds the first occurrence of the substring
					 ret = strstr(doc_list, selected_spec);
					 if (ret != NULL) {

						//is_atleast_one gets true if there is the doctor exists
						 is_atleast_one = true;
						 for (int i = 0; doc_list[i] != '\0'; i++) {
							 if ((doc_list[i] == ',') && (comma_count < 2)){
								 comma_count++;
								 if (comma_count == 1) {
									 buff_temp[i] = ' ';
									 continue;
								 }
								 if (comma_count == 2) {
									 buff_temp[i] = '\0';
									 break;
								 }
							 }
							 buff_temp[i] = doc_list[i];
						 }
						 comma_count = 0;

						//buff_temp -> store doctor's ID and Name only
						 snprintf(line, sizeof(line), "%s\n", buff_temp);
						 n = strlen(line);
				 		 Writen(sockfd, line, n); //write line
					 }
				}
				fclose(fp_spec);

				//If not found error generates and exit
				if(!is_atleast_one) {
					snprintf(line, sizeof(line), "Sorry! specialization not found!\n");
					n = strlen(line);
					Writen(sockfd, line, n);
					exit(0);
				}
				for ( ; ; ) {

						if ((n = Readline(sockfd, line, MAXLINE)) == 0)
							return;		/* connection closed by other end */

						//[Refered linke]https://regex101.com/
						//accept_code validation as given format
						if (sscanf(line, "%[a] %s %d", &confirmation_a , &selected_doc_Id, &selected_time_slot) == 3)
						{
							//accept code further validation (length)
							if((selected_time_slot < 4) && (selected_time_slot > 0) && (strlen(confirmation_a) < 2))
							{
								//start to read the file line by line, buff is a line of the text file
								fp = fopen("database.txt", "r");

								while(!feof(fp))
								{
									fgets(buff,255,fp);

									//To determine the end of the file in the middle of the loop
									//otherwise it may print last line twice
									//[Refered linke]https://stackoverflow.com/questions/29143338/c-program-prints-the-last-line-twice-file-i-o
									if (feof(fp))
										break;

									//check the selected_time_slot is free from the db
									//search for the selected_doc_Id is exists on the db
									if(strstr(buff, selected_doc_Id) != NULL)
									{
										if ((selected_time_slot == 1) && ((buff[strlen(buff)-6]) == '1'))
											is_available = true;
										else if((selected_time_slot == 2) && ((buff[strlen(buff)-4]) == '1'))
											is_available = true;
										else if((selected_time_slot == 3) && ((buff[strlen(buff)-2]) == '1'))
											is_available = true;
										else
											is_available = false;
									}
									else
										snprintf(line, sizeof(line), "Please check the doctor ID again!\n");
								}
								//[Refered linke]https://stackoverflow.com/questions/32366665/resetting-pointer-to-the-start-of-file
								//get back to the first line of the text file
								rewind(fp);
								fclose(fp);

								if (is_available)
								{
									snprintf(line, sizeof(line), "Doctor is available.Press 'c' to confirm the appointment.\n");
									n = strlen(line);
									Writen(sockfd, line, n);
									for ( ; ; ) {

										if ((n = Readline(sockfd, line, MAXLINE)) == 0)
											return;

											if (sscanf(line, "%[cC] %s %d", &confirmation , &doc_id_c, &time_c) == 3)
											{
												//[Refered linke]https://stackoverflow.com/questions/40605075/how-to-compare-2-character-arrays
												if((strcmp(selected_doc_Id, doc_id_c) == 0) && (time_c == selected_time_slot))
												{
															//Further validation
															if(strlen(confirmation) < 2)
															{
																	snprintf(line, sizeof(line), "Your appointment process successfully!\n\n");

																	//[Refered linke] https://codeforwin.org/2018/02/c-program-replace-specific-line-a-text-file.html
																	//Remove extra new line character from stdin
															    fflush(stdin);
															    //Open all required files
															    fPtr  = fopen("database.txt", "r");
															    fTemp = fopen("replace.tmp", "w");

															    //fopen() return NULL if unable to open file in given mode.
															    if (fPtr == NULL || fTemp == NULL)
															    {
															        //Unable to open file hence exit
																			snprintf(line, sizeof(line), "File access error! Try little while.\n");
																			n = strlen(line);
																			Writen(sockfd, line, n);
																			exit(0);
															    }

																	while(!feof(fPtr))
																	{
																		fgets(text_line, 255 , fPtr);

																		//To determine the end of the file in the middle of the loop
																		//otherwise it may print last line twice
																		//[Refered linke]https://stackoverflow.com/questions/29143338/c-program-prints-the-last-line-twice-file-i-o
																		if (feof(fPtr))
																			break;

																			//increse line count to find which line has the selected doctor ID
																			line_no++;

																			if(strstr(text_line, selected_doc_Id) != NULL)
																			{
																				if(selected_time_slot == 1)
																				{
																					text_line[strlen(text_line)-6] = '0';
																					break;
																				}
																				else if(selected_time_slot == 2)
																				{
																					text_line[strlen(text_line)-4] = '0';
																					break;
																				}
																				else if(selected_time_slot == 3)
																				{
																					text_line[strlen(text_line)-2] = '0';
																					break;
																				}
																			}
																	}
																	rewind(fPtr);

															    count = 0;
																	//[Refered linke] https://codeforwin.org/2018/02/c-program-replace-specific-line-a-text-file.html
																	//Read line from source file and write to destination
																	//file after replacing given line.
															    while ((fgets(buffer, 255, fPtr)) != NULL)
															    {
															        count++;
															        //If current line is line to replace
															        if (count == line_no)
																				fputs(text_line, fTemp);
															        else
															            fputs(buffer, fTemp);
															    }

															    //Close all files to release resource
															    fclose(fPtr);
															    fclose(fTemp);

															    //Delete original source file
															    remove("database.txt");

															    //Rename temporary file as original file
															    rename("replace.tmp", "database.txt");

																	n = strlen(line);
																	Writen(sockfd, line, n);
																	exit(0);
															}
															else
																snprintf(line, sizeof(line), "Check your confirmation code again!\n");
												}
												else
													snprintf(line, sizeof(line), "Confirmation code missmatch\n");
											}
											else
												snprintf(line, sizeof(line), "input error\n");

											n = strlen(line);
											Writen(sockfd, line, n); //write line
									}
								}
								else
								{
									snprintf(line, sizeof(line), "Sorry,Doctor is not available today.Please try another day.\n");
									n = strlen(line);
									Writen(sockfd, line, n);
									exit(0);
								}

							}
							else
								snprintf(line, sizeof(line), "Accept code does not meet the correct format!\n");
						}
						else
							snprintf(line, sizeof(line), "accept code error!\n");

						n = strlen(line);
						Writen(sockfd, line, n); //write line
					}
			}
			else
				snprintf(line, sizeof(line), "Specialization code error!\n");
		}
		else
			snprintf(line, sizeof(line), "Specialization code error!\n");

		n = strlen(line);
		Writen(sockfd, line, n); //write line
	}
}
