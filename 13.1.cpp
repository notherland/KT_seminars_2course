#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


void
proc_info (pid_t pid)
{
  printf ("PID: %u\n", getpid ());
  printf ("PPID: %u\n", getppid ());

  return;
}

int
main ()
{
  pid_t cpid, w;
  pid_t ppid = getpid ();
  int status;
  cpid = fork ();
  if (cpid == -1)
    {
      perror ("fork");
      exit (EXIT_FAILURE);
    }

  if (cpid == 0)
    {				/* Код, выполняемый потомком */
      printf ("\nChild process: \n");
      proc_info (getpid ());
    }
  else
    {				/* Код, выполняемый родителем */
      w = waitpid (cpid, &status, 0);
      if (w == -1)
	{
	  perror ("waitpid");
	  exit (EXIT_FAILURE);
	}
      printf ("\nParent process: \n");
      proc_info (getpid ());
      exit (EXIT_SUCCESS);
    }
  return 0;
}
