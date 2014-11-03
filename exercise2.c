/* This program assumes the parent writing to the pipe
 * and the child reading from the pipe */
#include "lab1.h"
#include <sys/wait.h>

int main(void)
{
	pid_t pid;
	int pfd[2];
	char *msgbuf;
	ssize_t n;

	if (pipe(pfd) == -1)
		err_sys("Cannot create pipe!");

	if ((pid = fork()) < 0) {
		err_sys("Cannot fork new process");
	} else if (!pid) {
		close(pfd[1]);
		printf("This is the child process, pid: %d\n", getpid());
		msgbuf = (char *) malloc(BUFSIZ);
		if (read(pfd[0], msgbuf, BUFSIZ) < 0)
			err_sys("Error on reading from pipe");
		printf("%s\n", msgbuf);
		exit(0);
	} else {
		close(pfd[0]);
		msgbuf = (char *) malloc(BUFSIZ);
		sprintf(msgbuf, "This is the message from %d", getpid());
		n = strlen(msgbuf) + 1;
		if (write(pfd[1], msgbuf, n) != n)
			err_sys("Error on writing to pipe");
		printf("This is the parent process, pid: %d\n", getpid());

		if (waitpid(pid, NULL, 0) < 0)
			err_sys("Failed to fetch child process termination status");
	}

	return 0;
}
