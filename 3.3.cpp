#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <cerrno>

const size_t BLOCK_SZ = 4096; // 4K

bool fcopy(int src_fd, int dst_fd);
bool dircopy(const char* src, const char* dst);
bool copy (const char* src, const char* dst);

bool fcopy(int src_fd, int dst_fd) { //copying of file
    ssize_t res = 0;
    struct stat st;
    ssize_t written = 0, count = 0, readded = 0;

    if (fstat(src_fd, &st) == -1) {
        perror("Failed fstat");
        return false;
    }

    void *str_src = (void *) malloc(BLOCK_SZ);

    while ((readded = read (src_fd, str_src, BLOCK_SZ)) > 0)
    {
        written = 0;

        while (written < readded)
        {
            res = write (dst_fd, str_src + written, readded - written);

            if (res < 0){
                perror ("Failed to write\n");
                return false;
            }
            written += res;
        }
    }
    if (readded < 0){
        printf ("Failed to read file\n");
    }

    //printf ("%d\n", st.st_mode);
    if (fchmod (dst_fd, st.st_mode) == -1){
    	perror("fchmod");
    	return false;
    }
   
    struct timespec times[2] = {st.st_atime, st.st_mtime};


    if (futimens(dst_fd, times) == -1){
    	perror("futimens");
    	return false;
    }

    free (str_src);
    close (src_fd);
    close (dst_fd);

    return true;
}


bool dircopy(const char* src, const char* dst){//copying of directory
	struct stat st;
	if (lstat(src, &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

	DIR *src_dir = opendir(src);
    if (src_dir == NULL){
        perror("Failed to open src directory " + *src);
		return false;
        }
    std::string dst_dir_name = ((std::string)dst + "/" + (std::string)src);

    const char * dst_dir_name_c = dst_dir_name.c_str();

            
    int dst_dir_fd = mkdir(dst_dir_name_c, st.st_mode);
    DIR *dst_dir = opendir(dst_dir_name_c);
    if (dst_dir == NULL){
        perror("Failed to open dst directory " + *dst);
        return false;
    }

  	struct dirent *next_dir;

    errno = 0;
    if ((next_dir = readdir (src_dir)) == NULL){
        if (errno != 0){
            perror("readdir");
            return false;
        }
    	printf ("End of stream\n");
    	return false;
    }
    
    const char* next_dir_name = next_dir->d_name;
	//std::cout << next_dir_name << " " << dst << std::endl;
	return copy(next_dir_name, dst_dir_name_c);
}


bool copy (const char* src, const char* dst)
{
    struct stat st, sb;
    int src_fd = -1, dst_fd = -1;

    if (lstat(src, &st) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

  
  	//std::cout << "src : " << src << std::endl << "dst : " << dst << std::endl;
    printf("File type: ");

    switch (st.st_mode & S_IFMT) {
        case S_IFBLK:  {printf("block device\n");            return true;};
        case S_IFCHR:  {printf("character device\n");        return true;};
        case S_IFDIR:  {
            printf("directory %s\n", src);
            
            return dircopy(src, dst);
        }
        case S_IFIFO:  {printf("FIFO/pipe\n");               return true;}
        case S_IFLNK:  {printf("symlink\n");                 return true;}
        case S_IFREG:  {
            printf("regular file\n");

            int src_fd = -1, dst_fd = -1;

            if ((src_fd = open (src, O_RDONLY)) < 0){
                perror ("Failed to open src\n");
                return false;}

            std::string dst_name = ((std::string)dst + "/" + (std::string)src);
			const char * dst_name_c = dst_name.c_str();   
			//std::cout << src << " " << dst_name << std::endl;

            if ((dst_fd = open (dst_name_c, O_WRONLY | O_TRUNC | O_CREAT, 0600)) < 0){
                perror ("Failed to open dst\n");
                close (src_fd);
                return false;}
                
            return fcopy(src_fd, dst_fd);

        }
        case S_IFSOCK: {printf("socket\n");                   return true;}
        default:       {printf("unknown?\n");                 return true;}}

}
//set umask 0777 or 0000
//do not copy src to src
//do not copy if directory with a same name directory is already exists

    int main(int argc, char *argv[]) {
        if (argc < 3) {
            printf("Too few arguments\n");
            return 1;
        }


        //printf ("%d %s %s", argc, argv[1], argv[2]);

        if (copy (argv[1], argv[2]))
    		printf ("Copied successfully");
        else
            printf ("Copied wrongly");

        return 0;
    }