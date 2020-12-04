#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

const long MQ_MSGSIZE = 4096;


int main(int argc, char *argv[]){
	if (argc < 3){
		printf ("Too few arguments");
		exit(0);
	}
	printf ("%s\n", argv[1]);
	mqd_t q = mq_open(argv[1], O_WRONLY);
	if (q == -1){
		perror("mq_open");
		return 1;
	}

	size_t msg_len = strlen(argv[2]);
	mq_send(q, argv[2], msg_len, 1);
	mq_close(q);
}