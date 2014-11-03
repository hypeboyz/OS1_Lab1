/* This program assumes the parent writing to the shared memory
 * and the child reading from the shared memory
 * Since the exercise doesn't define how to synchronize, there're
 * mutiple ways to achieve the expected result apart from my
 * implementation based on semaphore.
 * 1. We may simply make the child write to the shared memory and
 * make the parent waiting for the child to terminate, then print
 * the memory.
 * 2. By signal handling with sigaction(2) and sigsuspend(3).
 * 3. By advisory lock as the shared memory objects are file after
 * all. However this object is located in /dev/shm/ or some sort.
 * So this method should not be used if we take portability into
 * consideration.
 */
#include "lab1.h"
#include <sys/wait.h>
#include <mqueue.h>
#include <semaphore.h>
#include <sys/mman.h>

#define PAGE_SIZE 4096
#define MAPSIZ (PAGE_SIZE * 32)

int main(int argc, char *argv[])
{
	pid_t pid;
	char *msgbuf;
	size_t buflen;
	int shmid;
	sem_t *semobj;
	char *shmname;
	char *semname;
	char *shmp;

	if ((shmname = path_alloc(NULL)) == NULL)
		err_sys("Cannot allocate memory for path name");

	if (argc == 1)
		strcpy(shmname, "/shmobj");
	else if (argc == 2)
		strncpy(shmname, argv[1], strlen(argv[1]) + 1);
	else
		err_quit("Usage: ./exercise3 (msg_path)");
	if (shmname[0] != '/')
		err_quit("Message queue path name must start with '/'");

	if ((shmid = shm_open(shmname, O_RDWR | O_CREAT, FILE_MODE)) == -1)
		err_sys("Cannot open message queue");
	if (ftruncate(shmid, MAPSIZ) == -1)
		err_sys("Cannot truncate the file");
	if ((shmp = mmap(NULL, MAPSIZ, PROT_READ | PROT_WRITE, MAP_SHARED, shmid,
				0)) == NULL)
		err_sys("Cannot map memory to shmobj");

	/* Fetch the allocated size in case for the buffer overrun
	 * with the following strncpy(3)
	 * I don't intend to expose the possiblity for users to define
	 * the path of semaphore as they should not be so interested
	 * in how the program was synchronized :-) */
	if ((semname = path_alloc(&buflen)) == NULL)
		err_sys("Cannot allocate memory for semaphore path");
	strncpy(semname, "/tmpsem", buflen);
	if ((semobj = sem_open(semname, O_RDWR | O_CREAT, FILE_MODE, 1)) == SEM_FAILED)
		err_sys("Cannot get the semaphore object!");
	if (sem_init(semobj, !0, 1) == -1)
		err_sys("Cannot initialize semaphore instance");

	if (sem_wait(semobj) == -1)
		err_sys("sem_wait() error");

	if ((pid = fork()) < 0) {
		err_sys("Cannot fork new process");
	} else if (!pid) {
		printf("This is the child process, pid: %d\n", getpid());
		/* Well, this sem_open() seems to be redundent since
		 * the entire memory space was copied by fork(). But
		 * problems may be caused due to copy on write when I changed
		 * value of the semaphore. Keeping this sem_open() would have
		 * spared effort dealing with another shmget(2) and copying
		 * semaphore instance into that shared memory */
		if ((semobj = sem_open(semname, O_RDWR)) == SEM_FAILED)
			err_sys("Cannot get the semaphore object!");
		if (sem_wait(semobj) == -1)
			err_sys("sem_wait() error");

		printf("%s\n", shmp);

		if (sem_post(semobj) == -1)
			err_sys("sem_post() error");
		sem_close(semobj);
		exit(0);
	} else {
		printf("This is the parent process, pid: %d\n", getpid());
		msgbuf = (char *) malloc(BUFSIZ);
		sprintf(msgbuf, "This is the message from %d", getpid());
		strcpy(shmp, msgbuf);

		if (sem_post(semobj) == -1)
			err_sys("sem_post() error");

		if (waitpid(pid, NULL, 0) < 0)
			err_sys("Failed to fetch child process termination status");
		sem_close(semobj);
	}

	sem_unlink(semname);
	shm_unlink(shmname);
	return 0;
}
