#include	"utils.h"		/* Header file */
//[Refered file] This file is referd and mainly based on Practical 5 of DCCN3 module in (SLIIT)Srilanka institute of information technology.
void
str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];

	while ((void *) Fgets(sendline, MAXLINE, fp) != NULL) {

	if (Readline(sockfd, recvline, MAXLINE) == 0)
			err_quit("str_cli: server terminated prematurely");

	Fputs(recvline, stdout);

	Writen(sockfd, sendline, strlen(sendline));

	}
}
