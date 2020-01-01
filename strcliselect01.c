#include	"utils.h"		/* Header file */
//[Refered file] This file is referd and mainly based on Practical 5 of DCCN3 module in (SLIIT)Srilanka institute of information technology.
void
str_cli(FILE *fp, int sockfd)
{
	int		maxfdp1;
	fd_set		rset; //file discriptor set (we are worring about reading perpouse so only read set is enough)
	char		sendline[MAXLINE], recvline[MAXLINE];

	FD_ZERO(&rset);
	for ( ; ; )
  {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd , &rset); //first param: -name of the client's socket

		maxfdp1 = max(fileno(fp), sockfd) + 1; //arrays are starting with 0, thats why we do this
		select(maxfdp1,&rset,NULL,NULL,NULL); //wait forver until we type something

		if (FD_ISSET(sockfd , &rset)) //check for socket file discriptor got data
        	{
			/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		}

		if (FD_ISSET(fileno(fp) , &rset)) //this is good rather than using if-else
        	{
			/* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
