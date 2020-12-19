#include <sys/statvfs.h>
#include <iostream>
#include <unistd.h>

using namespace std;

const long GB = (1 << 30);

int
main (int argc, char *argv[])
{
  if (argc != 2)
    {
      cout << "Usage: " << argv[0] << " file_name\n" << endl;
      return 0;
    }

  struct statvfs st;

  if (statvfs (argv[1], &st) != 0)
    {
      perror ("statvfs");
      return EXIT_FAILURE;
    }

  long block_size = st.f_frsize;
  cout << "block size " << block_size << endl;
  cout << "total data blocks in file system " << st.f_blocks <<
    "; total space " << st.f_blocks * block_size / GB << endl;
  cout << "free blocks in fs " << st.
    f_bfree << "; free space " << st.f_bfree * block_size / GB << endl;
  cout << "free blocks availible " << st.f_bavail << "; availible space " <<
    st.f_bfree * block_size / GB << endl;

  return 0;
}
