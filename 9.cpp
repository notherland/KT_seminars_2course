#include <sys/statfs.h>
#include <iostream>
#include <unistd.h>

using namespace std;

const long Gb = 1073741824;

int main (int argc, char *argv[])
{
	if (argc < 2)
	{
		cout << "Too few arguments";
		return 0;
	}

	struct statfs st;

	if (statfs (argv[1], &st) != 0)
	{
		perror ("statfs");
		return 0;
	}

	long block_size = st.f_bsize;
	cout << "block size " << block_size << endl;
	cout << "total data blocks in file system " << st.f_blocks << "; total space" << st.f_blocks * block_size / Gb << endl;
	cout << "free blocks in fs " << st.f_bfree << "; free space " << st.f_bfree * block_size / Gb << endl;
	cout << "free blocks availible " << st.f_bavail << "; availible space " << st.f_bfree * block_size / Gb<< endl;
	
	return 0;
}