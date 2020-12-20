#include "Shmem.h"

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      printf ("Usage: %s message\n", argv[0]);
      return 1;
    }

  int shmid = shm_open (SHMEM_NAME, O_RDWR | O_TRUNC | O_CREAT, 0600);
  if (shmid == -1)
    {
      perror ("shm_open");
      shm_unlink (SHMEM_NAME);
      return 1;
    }

  if (ftruncate (shmid, (off_t) SHMEM_SIZE) == -1)
    {
      perror ("ftruncate");
      return 2;
    }

  void *addr;
  struct buf_t *buffer;
  sem_init (&buffer->sem, 0, 1);
  buffer->message = (char *) malloc (SHMEM_SIZE);
  if (buffer->message == NULL)
    {
      printf ("Failed to allocate memory\n");
      return 3;
    }
  strcpy (buffer->message, argv[1]);


  while (1)
    {
      addr =
	mmap (NULL, SHMEM_SIZE + sizeof (sem_t), PROT_WRITE, MAP_SHARED,
	      shmid, 0);
      if (addr == MAP_FAILED)
	{
	  perror ("mmap");
	  shm_unlink (SHMEM_NAME);
	  return 4;
	}
      if (sem_wait (&buffer->sem) == 0)
	{
    memcpy (addr, &buffer, sizeof (sem_t) + SHMEM_SIZE);
    sem_post (&buffer->sem);
  }
      sleep (1);
    }
    free (buffer->message);
  munmap (addr, SHMEM_SIZE);
  shm_unlink (SHMEM_NAME);
  return 0;
}
