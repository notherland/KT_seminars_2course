#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>

const char *SHMEM_NAME = "/SHMEM_NAME";
enum {SHMEM_SIZE = 4096};

struct buf_t
{
  sem_t sem;
  char *message;
};
