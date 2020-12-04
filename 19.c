#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <mcheck.h>
#include <stdio.h>

int shar = 0;
pthread_mutex_t mutex;

void *increase(void *arg){
	int i = *((int *)arg);
	while (shar < 100){
	pthread_mutex_lock(&mutex);
	printf ("Process %d  %d\n", i, shar);
	shar++;
	pthread_mutex_unlock(&mutex);
	sleep(0.02);
	}
	return NULL;
}

int main(){
	pthread_t threads[5];
	int progress = 0;

	pthread_mutex_init (&mutex, NULL);
	
	for (int i = 0; i < 5; i++){
		printf ("PROCESS %d CREATED\n", i);
		pthread_create(&threads[i], NULL, increase, &i);
	}

	for (int i = 0; i < 5; i++)
    {
    	pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);



	return 0;	
}