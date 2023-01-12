#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <semaphore.h>
#define MAX_RESOURCES 5

sem_t semafor;
pthread_mutex_t mtx;
pthread_t *th;
int n;

void barrier_point()
{
	pthread_mutex_lock(&mtx);
	--n;
	if(n==0)
	{
		pthread_mutex_unlock(&mtx);
		sem_post(&semafor);
		return;
	}
	pthread_mutex_unlock(&mtx);
	sem_wait(&semafor);
	sem_post(&semafor);
}

void* thread_routine( void* x)
{
	int*c= (int*) x;
	int count= *c;
	printf("%d reached the barrier\n", count);
	barrier_point();
	printf("%d passed the barrier\n", count);
	free(c);
	return NULL;
}

int main()
{
	n=MAX_RESOURCES;
	th=(pthread_t*)malloc(MAX_RESOURCES* sizeof(pthread_t));
	int sem=0;
	if(sem_init(&semafor,0,sem))
	{
		perror(NULL);
		return errno;
	}	
	
	if(pthread_mutex_init(&mtx, NULL))
	{
		perror(NULL);
		return errno;
	}
	
	for(int i=0;i<5; ++i)
	{
		int* c;
		c=(int*) malloc(sizeof(int));
		*c=i;
		if(pthread_create(&th[i],NULL,thread_routine, c))
		{
			perror(NULL);
			return errno;
		}
	}
	
	for(int i=0;i<5;++i)
	{
		if(pthread_join(th[i],NULL))
		{
			perror(NULL);
			return errno;
		}
	}
	
	if(sem_destroy(&semafor))
	{
		perror(NULL);
		return errno;
	}
	
	if(pthread_mutex_destroy(&mtx))
	{
		perror(NULL);
		return errno;
	}
	
	free(th);
	return 0;
}