#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <mcheck.h>
#include <stdio.h>
#include <time.h>

volatile int shar = 0;
const int NUM_THREADS = 5;

void *
increase (void *arg)
{
  pthread_mutex_t mutex = *((pthread_mutex_t *) arg);
  struct timespec time = { 0, 2000000 };
  while (shar < 100)
    {
      pthread_mutex_lock (&mutex);
      shar++;
      pthread_mutex_unlock (&mutex);
      nanosleep (&time, NULL);
    }
}

int
main ()
{
  pthread_t threads[NUM_THREADS];
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  struct timespec time = { 0, 2000000 };
  int real_num_threads = 0;

  for (real_num_threads = 0; real_num_threads < NUM_THREADS;
       real_num_threads++)
    {
      if (pthread_create (&threads[real_num_threads], NULL, increase, &mutex)
	  != 0)
	{
	  perror ("pthread_create");
	  real_num_threads--;
	  break;
	}
      printf ("THREAD %d CREATED\n", real_num_threads);
    }

  while (shar < 100)
    {
      printf ("%d\n", shar);
      nanosleep (&time, NULL);
    }

  for (int k = 0; k < real_num_threads; k++)
    {
      if (pthread_join (threads[k], NULL) != 0)
	{
	  perror ("pthread_join");
	}
      printf ("THREAD %d ENDED\n", k);
    }

  pthread_mutex_destroy (&mutex);

  return 0;
}
