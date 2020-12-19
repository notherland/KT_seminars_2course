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

  char *addr;
  int ver = 0;
  int length = strlen (argv[1]);


  while (1)
    {
      if (ver == MAXINT - 2)

	printf ("ver = %d\n", ver);
      addr = mmap (NULL, SHMEM_SIZE, PROT_WRITE, MAP_SHARED, shmid, 0);
      if (addr == MAP_FAILED)
	{
	  perror ("mmap");
	  shm_unlink (SHMEM_NAME);
	  return 3;
	}

      ver++;
      memcpy (addr, &ver, 1);
      printf ("%s\n", addr);
      memcpy (addr + 1, argv[1], length);
      printf ("%s\n", addr);
      ver++;
      memcpy (addr, &ver, 1);
      printf ("%s\n", addr);
      sleep (1);
    }
  munmap (addr, SHMEM_SIZE);
  shm_unlink (SHMEM_NAME);
  return 0;
}
