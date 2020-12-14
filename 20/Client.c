#include "Shmem.h"

int main(){

	int shmid = shm_open(SHMEM_NAME, O_RDWR, 0);
	if (shmid == -1){
		perror("shm_open");
		return 1;
	}

	if (ftruncate(shmid, (off_t) SHMEM_SIZE) == -1)
    {
        perror("ftruncate");
        shm_unlink(SHMEM_NAME);
        return 2;
    }

    int prevver = 0;
	char *buffer = malloc(SHMEM_SIZE);
	if (buffer == NULL){
		perror("malloc");
		shm_unlink(SHMEM_NAME);
		return 3;
	}

	while (1){
		buffer = (char *)mmap(NULL, 1, PROT_READ, MAP_SHARED, shmid, 0);
		if (buffer == MAP_FAILED){
			perror("mmap");
			shm_unlink(SHMEM_NAME);\
			free(buffer);
			return 4;
		}

		if (prevver == *buffer){	
			printf ("no\n");
			sleep(1);
			continue;
		}

		prevver = *buffer;

		buffer = (char *)mmap(NULL, SHMEM_SIZE, PROT_READ, MAP_SHARED, shmid, 0);
		if (buffer == MAP_FAILED){
			perror("mmap");
			shm_unlink(SHMEM_NAME);
			free(buffer);
			return 4;
		}
		
		if (prevver != *buffer){	
			printf ("changed\n");
			prevver = *buffer;
			continue;
		}

		printf ("%s\n", buffer);
	}

	free(buffer);
	munmap(addr, SHMEM_SIZE);
	shm_unlink(SHMEM_NAME);
	return 0;
}