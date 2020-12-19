#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/types.h>

const char *MQ_NAME = "/queue";

int
main ()
{
  struct mq_attr st;
  mqd_t q = mq_open (MQ_NAME, O_RDWR | O_CREAT, 0666, NULL);

  if (q == -1)
    {
      perror ("mq_open");
      return 1;
    }

  if (mq_getattr (q, &st) == -1)
    {
      perror ("mq_getattr");
      mq_close (q);
      return 1;
    }

  printf ("Flags: %ld\n", st.mq_flags);
  printf ("Max messages count: %ld\n", st.mq_maxmsg);
  printf ("Max message size: %ld\n", st.mq_msgsize);
  printf ("Count of messages in the queue: %ld\n", st.mq_curmsgs);

  mq_unlink (MQ_NAME);
  mq_close (q);
  return 0;
}
