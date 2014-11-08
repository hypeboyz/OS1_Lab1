/* Exercise 3
 * Author: Chen, Yiqun Gao, Ziheng Yu, Chunyuan
 */
/* This program assumes the parent writing to the message queue
 * and the child reading from the message queue */
#include "lab1.h"
#include <sys/wait.h>
#include <mqueue.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	char *msgbuf;
	ssize_t buflen;
	mqd_t mqid;
	char *mqname;
	struct mq_attr mq_attribute;

	if ((mqname = path_alloc(NULL)) == NULL)
		err_sys("Cannot allocate memory for path name");

	if (argc == 1)
		strcpy(mqname, "/msgqueue");
	else if (argc == 2)
		strncpy(mqname, argv[1], strlen(argv[1]) + 1);
	else
		err_quit("Usage: ./exercise3 (msg_path)");
	if (mqname[0] != '/')
		err_quit("Message queue path name must start with '/'");

	if ((mqid = mq_open(mqname, O_RDWR | O_CREAT, FILE_MODE, NULL)) == -1)
		err_sys("Cannot open message queue");

	if (mq_getattr(mqid, &mq_attribute) == -1)
		err_sys("Cannot fetch the attribute information of queue");

	buflen = mq_attribute.mq_msgsize;

	if ((pid = fork()) < 0) {
		err_sys("Cannot fork new process");
	} else if (!pid) {
		printf("This is the child process, pid: %d\n", getpid());
		msgbuf = (char *) malloc(buflen);
		if (mq_receive(mqid, msgbuf, buflen, NULL) == -1)
			err_sys("Cannot fetch message from queue");
		printf("%s\n", msgbuf);
		mq_close(mqid);
		exit(0);
	} else {
		printf("This is the parent process, pid: %d\n", getpid());
		msgbuf = (char *) malloc(buflen);
		sprintf(msgbuf, "This is the message from %d", getpid());
		if (mq_send(mqid, msgbuf, buflen, 0) == -1)
			err_sys("Cannot send message to queue");

		if (waitpid(pid, NULL, 0) < 0)
			err_sys("Failed to fetch child process termination status");
	}

	mq_close(mqid);
	mq_unlink(mqname);
	return 0;
}
