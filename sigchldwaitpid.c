#include	"utils.h"		/* Header file */
#include	<sys/wait.h>

//[Refered file] This file is referd and mainly based on Practical 5 of DCCN3 module in (SLIIT)Srilanka institute of information technology.
void
sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}
