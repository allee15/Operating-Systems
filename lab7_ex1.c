#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#define MAX_RESOURCES 5

int available_resources= MAX_RESOURCES;
pthread_mutex_t mtx;
pthread_t *th;

int decrease_count(int count)
{
	if (available_resources < count)
		{pthread_mutex_lock(&mtx);
		return -1;}
	else 
		{available_resources -= count;
		printf("Released %d resources %d remaining\n",available_resources,count);
		pthread_mutex_unlock(&mtx);
		}
	return 0;
}

int increase_count(int count)
{
	pthread_mutex_lock(&mtx);
	available_resources+= count;
	printf("Got %d resources %d remaining\n",available_resources,count);
	pthread_mutex_unlock(&mtx);
	return 0;
}

void* thread_routine( void* x)
{
	int*c= (int*) x;
	int count= *c;
	if(!decrease_count(count))
		increase_count(count);
	free(c);
	return NULL;
}

int main()
{
	th = (pthread_t*)malloc(MAX_RESOURCES* sizeof(pthread_t));
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
	
	if(pthread_mutex_destroy(&mtx))
	{
		perror(NULL);
		return errno;
	}
	
	free(th);
	return 0;
}