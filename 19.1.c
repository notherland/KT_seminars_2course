#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>

const char *SEM_NAME = "/semaphore";
volatile int shar = 0;
const int NUM_THREADS = 5;

void *Thread1(void *arg){
	sem_t sem = *((sem_t *)arg);
	struct timespec time = {0, 200000000000};
	while (shar < 100){
		sem_wait(&sem);
		shar++;
		printf("process 1, shar = %d\n", shar);
		nanosleep(&time, NULL);
		sem_post(&sem);
	}
	return NULL;
}

void *Thread2(void *arg){
	sem_t sem = *((sem_t *)arg);
	struct timespec time = {0, 200000000000};
	while (shar < 100){
		sem_wait(&sem);
		shar++;
		printf("process 2, shar = %d\n", shar);
		nanosleep(&time, NULL);
		sem_post(&sem);
	}
	return NULL;
}

int main(){
	int real_num_threads = 2;
	sem_t sem;
	struct timespec time = {0, 2000};
	pthread_t thread1;
    pthread_t thread2;

	 sem_init(&sem, 0, 1);
	
	if (pthread_create(&thread1, NULL, Thread1, &sem) != 0){
		perror("pthread_create");
		real_num_threads--;
	}
    if (pthread_create(&thread2, NULL, Thread2, &sem) != 0){
		perror("pthread_create");
		real_num_threads--;
	}

	if (pthread_join(thread1, NULL) != 0){
			perror ("pthread_join");
		}
	printf ("THREAD 1 ENDED\n");
	if (pthread_join(thread2, NULL) != 0){
			perror ("pthread_join");
		}
	printf ("THREAD 2 ENDED\n");

	sem_destroy(&sem);

	return 0;	
}