#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

int main(void) 
{
	int pid;
	if (pid = fork()) {
		wait(0);
	}
	else {
		execlp("ls", "ls", "-l", "-a", "/usr/", NULL);
	}
	
	return 0;
}