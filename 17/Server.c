#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

const long MQ_MSGSIZE = 4096;


int main(){
	struct mq_attr *st;
	char *msg = (char *) malloc(MQ_MSGSIZE);
	int sz = -1, *prior;

	st->mq_flags = 0;
	st->mq_maxmsg = 10;
	st->mq_msgsize = MQ_MSGSIZE;
	st->mq_curmsgs = 0;
	
	mqd_t q = mq_open("/q", O_RDWR | O_CREAT, 0620, st);
	if (q == -1){
		perror("mq_open");
		return 1;
	}

	while (1){
		if ((sz = mq_receive(q, msg, MQ_MSGSIZE, prior)) == -1){
			perror ("mq_recieve");
			continue;
		}

		printf ("%s\n", msg);
	}
}
