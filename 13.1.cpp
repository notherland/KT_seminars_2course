#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <pwd.h>
#include <stdlib.h>


void proc_info (pid_t pid)
{
	printf("PID: %u\n", getpid());
	printf("PPID: %u\n", getppid());

	return;
}



int main()
{
    pid_t cpid, w;
    pid_t ppid = getpid();
    int status;
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    printf("CPID: %u\n", cpid);
    printf("PID: %u\n", getpid());
    printf("PPID: %u\n", ppid);

    if (cpid == 0) {            /* Код, выполняемый потомком */
    printf("\nChild process: \n");
    proc_info(getpid());
    } else {                    /* Код, выполняемый родителем */
    do {
        w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
        if (w == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        printf("\nParent process: \n");
        proc_info(getpid());
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    exit(EXIT_SUCCESS);
}
}