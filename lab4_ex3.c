#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

void collatz(int argument)
{
	printf("%d", argument);
	while(argument > 1){
		if(argument & 1)
			argument = 3* argument +1;
		else
			argument = argument /2;
		printf("%d",argument);
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	printf("starting parent: %d\n", getpid());
	for(int i=1; i<argc; ++i){
		pid_t pid= fork();
		if(pid<0){
			perror("didn't fork\n");
			return errno;}
		else if(pid==0){
			printf("child id= %d\n", getpid());
			int argument = atoi(argv[i]);
			printf("For number %d\n", argument);
			collatz(argument);
			exit(0);}
	}
	for(int i=1; i<argc; ++i){
		wait(NULL);
		printf("done child with id= %d, parent id=%d\n", getpid(), getppid());
	}
	return 0;
}