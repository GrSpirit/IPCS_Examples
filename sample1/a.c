#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int pid = fork();	// Ответвление дочернего процесса
	srandom((unsigned int)time(NULL)+getpid());
	if ( pid < 0 ) {		// Проверка на ошибку 
		fprintf(stderr, "Failed to fork\n");
		exit (-1);
	} else if ( pid == 0 ) {	// Дочерний процесс
		usleep(random() % 1000);
		printf("Pid: %d. Parent: %d.\n", getpid(), getppid());
	} else {				// Родительский процесс
		usleep(random() % 1000);
		printf("Pid: %d. Child: %d.\n", getpid(), pid);
	}

	return 0;
}