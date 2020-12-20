#include "Shmem.h"

int
main ()
{

  int shmid = shm_open (SHMEM_NAME, O_RDWR, 0);
  if (shmid == -1)
    {
      perror ("shm_open");
      return 1;
    }

  if (ftruncate (shmid, (off_t) SHMEM_SIZE) == -1)
    {
      perror ("ftruncate");
      shm_unlink (SHMEM_NAME);
      return 2;
    }

  struct buf_t *buffer;

  while (1)
    {

      buffer =
	(struct buf_t *) mmap (NULL, SHMEM_SIZE, PROT_READ, MAP_SHARED, shmid,
			       0);
      if (buffer == MAP_FAILED)
	{
	  perror ("mmap");
	  shm_unlink (SHMEM_NAME);
	  return 4;
	}

      if (sem_wait (&buffer->sem) == 0)
	printf ("%s\n", buffer->message);
      sem_post (&buffer->sem);
      sleep (1);
    }

  munmap (buffer, SHMEM_SIZE);
  shm_unlink (SHMEM_NAME);
  return 0;
}
