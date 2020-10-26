#include <sys/file.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>




int main(){

	int counter = 0;

	int fd = open ("counter.txt", O_CREAT | O_RDWR, 0666);
	if (fd == -1)
	{
		printf ("Failed to open file");
		return 1;
	} 

	FILE *stream = fdopen(fd, "r+");
	if (stream == NULL)
	{
		perror("fdopen");
		close (fd);
		return 1;
	}

	if (flock (fd, LOCK_EX) == -1)
	{
		perror ("flock");
		close(fd);
		return 1;
	}

	fscanf (stream, "%d", &counter);
	rewind(stream);
	fprintf(stream, "%d", ++counter);

	flock (fd, LOCK_UN);
	fclose (stream);
	close (fd);
	return 0;
}
