#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>


int
main (int argc, char *argv[])
{
  if (argc != 3)
    {
      printf ("Usage: %s /mq_name text\n", argv[0]);
      return 1;
    }

  mqd_t q = mq_open (argv[1], O_WRONLY);
  if (q == -1)
    {
      perror ("mq_open");
      return 2;
    }

  if (mq_send (q, argv[2], strlen (argv[2]), 1) == -1)
    {
      perror ("mq_send");
    }

  mq_close (q);
  return 0;
}
