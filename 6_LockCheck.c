#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_THREADS 100

sem_t mutex;

/*Keeping the number of threads that have entered the critical section*/
int counter; 
/*In case the number of threads that entered the critical section is
greater that the number of threads that were created, we can assume 
that a deadlock has occured. */

struct timespec timestamp_counter, prev_timestamp;
/*prev_timestamp keeps track of the timestamp of the previous thread when it
entered the critical section,
time_elapsed computes the difference between the timestamp of the current thread 
and the previous thread, and compares it with a threshold value that you can set
to check for starvation.*/

void *thread_func(void *arg)
 {
    int thread_id = *((int *)arg);

    /* Acquire the semaphore */
    sem_wait(&mutex);

    /*Get the current timestamp*/
    clock_gettime(CLOCK_MONOTONIC, &timestamp_counter);

    printf("Thread %d entered critical section at %ld sec and %ld nsec\n", thread_id, timestamp_counter.tv_sec, timestamp_counter.tv_nsec);
    
    if (counter>0)
    {
        double time_elapsed = ((double)(timestamp_counter.tv_sec - prev_timestamp.tv_sec)) + ((double) (timestamp_counter.tv_nsec - prev_timestamp.tv_nsec) / 1e9);
        if (time_elapsed > threshold)
        {
            printf("Starvation detected. Time passed since last thread entered the critical section: %f sec\n", time_elapsed);
        }
    }
    prev_timestamp = timestamp_counter;
    counter++;

    /* Do some work */

    /* Release the semaphore */
    sem_post(&mutex);

    printf("Thread %d left critical section\n", thread_id);

    pthread_exit(NULL);
}

int main() 
{
    int i, thread_id[MAX_THREADS];
    pthread_t threads[MAX_THREADS];

    /* Initialize the semaphore */
    sem_init(&mutex, 0, 1);

    /* Create the threads */
    for (i = 0; i < MAX_THREADS; i++) 
    {
        thread_id[i] = i;
        pthread_create(&threads[i], NULL, thread_func, (void *)&thread_id[i]);
    }

    if (counter > MAX_THREADS)
        printf("A deadlock has occured\n");

    /* Wait for all threads to complete */
    for (i = 0; i < MAX_THREADS; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    /* Cleanup */
    sem_destroy(&mutex);

    return 0;
}
/*This example uses a semaphore to implement mutual exclusiom in the critical
section, which is the section of the code where shared resource is accessed.
Each thread acquires the semaphore before entering the critical section and
releases it after leaving it. This ensures that only one thread can be in the
critical section at a time, preventing race conditions.*/

/*To check for starvation you can use a timestamp counter to keep track of the 
time that each thread entered the critical section and compare it with the time 
that the next thread entered. If the time that passed is too long, you can assume
that starvation has occured.*/

/*The timestamp counter and the clock_gettime() function are used to get de current
timestamp in seconds and nanoseconds and get the time when each thread enters and leaves
the critical section.*/