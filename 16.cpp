#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/types.h>

int main(){
	struct mq_attr st;
	mqd_t q = mq_open("/queue", O_RDWR | O_CREAT, 0666, NULL);

	if (q == -1)
	{
		perror("mq_open");
		return 1;
	}

	if (mq_getattr(q, &st) == -1)
	{
		perror("mq_getattr");
		mq_close(q);
		return 1;
	}

	printf ("Flags: %ld\n", st.mq_flags);
	printf ("Max messages count: %ld\n", st.mq_maxmsg);
	printf ("Max message size: %ld\n", st.mq_msgsize);
	printf ("Current message: %ld\n", st.mq_curmsgs);

	while (mq_unlink("/queue") == 0)
	{}

mq_close(q);

return 0;
}