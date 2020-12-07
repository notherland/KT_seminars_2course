#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>


int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Usage: %s /mq_name\n", argv[0]);
		return 1;
	}
	
	mqd_t q = mq_open(argv[1], O_RDWR | O_CREAT, 0620, NULL);
	if (q == (mqd_t)-1){
		perror("mq_open");
		return 2;
	}

	struct mq_attr st;
	mq_getattr(q, &st);
	unsigned msg_len = st.mq_msgsize;
	char *msg = (char *) malloc(msg_len);
	if (!msg){
		mq_close(q);
		fprintf(stderr, "failed to allocate %ld bytes for msg\n", st.mq_msgsize);
		return 3;
	}
	
	unsigned priority;
	ssize_t sz;

	while (1){
		if ((sz = mq_receive(q, msg, msg_len, &priority)) == -1){
			if (errno == EMSGSIZE){
				mq_getattr(q, &st);
				msg_len = st.mq_msgsize;
				msg = (char *)realloc(msg, msg_len);
				continue;
			}
			if (errno == EINTR)
				break;
		}

		printf ("%.*s\n", (int)msg_len, msg);
	}
	mq_close(q);
	return 0;
}
