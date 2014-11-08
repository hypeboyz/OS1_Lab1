/* Exercise 1
 * Author: Chen, Yiqun Gao, Ziheng Yu, Chunyuan
 */
#include "lab1.h"
#include <sys/wait.h>

int main()
{
	pid_t pid;

	if ((pid = fork()) < 0) {
		err_sys("Cannot fork new process");
	} else if (!pid) {
		printf("This is the child process, pid: %d\n", getpid());
		exit(0);
	} else {
		printf("This is the parent process, pid: %d\n", getpid());

		if (waitpid(pid, NULL, 0) < 0)
			err_sys("Failed to fetch child process termination status");
	}

	return 0;
}
