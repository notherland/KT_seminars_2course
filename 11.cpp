#include <sys/file.h>


int main(){

	int fd = -1;
	if (fd = open ("counter.txt", O_CREATE, 0600) == -1)
	{
		printf ("Failed to open file");
		return 1;
	} 


}