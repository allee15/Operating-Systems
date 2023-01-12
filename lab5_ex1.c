#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <string.h>
#include <stdlib.h>
#define dim 4096
int main(int argc, char** argv) {
    char *shm_name = "collatz";
    int shm_fd ;
    shm_fd = shm_open ( shm_name , O_CREAT | O_RDWR , S_IRUSR | S_IWUSR );
    if ( shm_fd < 0) {
        perror ( NULL );
        return errno ;
    }

    size_t psize = getpagesize();
    size_t shm_size = 1024 * psize;

    if ( ftruncate ( shm_fd , shm_size ) == -1) {
        perror ( NULL );
        shm_unlink ( shm_name );
        return errno ;
    }

   for(int i = 1 ; i < argc; ++i) {
       pid_t pid = fork();
       if(pid < 0) {
           perror("Didn't fork!\n");
           return errno;
       } else if(pid == 0) {

           printf("Starting parent process\n");
           char* ceva = mmap (NULL , shm_size , PROT_WRITE , MAP_SHARED , shm_fd , (i-1)*dim);

           if ( ceva == MAP_FAILED ) {
               perror ( NULL );
               shm_unlink ( shm_name );
               return errno ;
           }

           int argument = atoi(argv[i]);
           ceva += sprintf(ceva, "For number %d", argument);
           while(argument > 1) {
               if(argument % 2) {
                   argument = argument*3 + 1;
               }
               argument /= 2;
               ceva += sprintf(ceva, " %d ", argument);
           }
           ceva += sprintf(ceva, "\n");
           printf("Done with child with id = %d, parent id = %d\n", getpid(), getppid());
           munmap(ceva, dim);
           exit(0);
       }
   }

   for(int i = 1; i < argc; ++i) {
       wait(NULL);
   }


    for(int i = 1; i < argc; ++i) {
        char* ceva = mmap(NULL , dim , PROT_READ , MAP_SHARED , shm_fd , (i-1)*dim);
        if ( ceva == MAP_FAILED ) {
            perror ( NULL );
            shm_unlink ( shm_name );
            return errno ;
        }
        printf("%s", ceva);
        munmap(ceva, dim);
    }
    shm_unlink(shm_name);
    munmap(0, getpagesize()*argc);
    return 0;
}