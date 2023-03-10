#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
void* reverse(void* args){
    int len = 0;
    char argument = (char) args;
    while(argument[len] != '\0'){
        ++len;
    }
    int result_len = 0;
    char* result = (char*) malloc(len * sizeof(char));
    for(int i = len - 1; i >= 0; --i){
        result[result_len] = argument[i];
        ++result_len;
    }
    return result;
}

int main(int argc, char* argv[]){
    pthread_t thread;
    char* argument = argv[1];
    void* retval;
    
    if(pthread_create(&thread, NULL, reverse, argument)){
        perror(NULL);
        return errno;
    }
    if(pthread_join(thread, &retval)){
        perror(NULL);
        return errno;
    }
    char* ret = (char*) retval;
    printf("Reversed string: %s\n", ret);
    
    free(ret);
    return 0;
}