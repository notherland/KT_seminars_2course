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

	if (pthread_mutex_init (&mutex, NULL) != 0)
	{
		perror ("pthread_mutex_init");
		exit(0);
	}
	
	for (int i = 0; i < 5; i++){
		if (pthread_create(&threads[i], NULL, increase, &i) != 0)
		{
			perror ("pthread_create");
			continue;
		}
		printf ("PROCESS %d CREATED\n", i);
	}

	for (int i = 0; i < 5; i++)
	{
		if (pthread_join(threads[i], NULL) != 0){
			perror ("pthread_join");
		}
	}

	pthread_mutex_destroy(&mutex);



	return 0;	
}