#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
struct thread_params{
    int line;
    int column;
};
int nrlin1, nrcol1, nrlin2, nrcol2;
int **m1, **m2, **result;
void* multiply(void* args){
    struct thread_params* index =  (struct thread_params*) args;
    int line = index->line;
    int column = index->column;
    free(index);
    int result = (int)malloc(sizeof(int));
    for(int k = 0;k < nrcol1; ++k)
        (*result) += m1[line][k] * m2[k][column];
    return result;
}

int main(int argc, char* argv[]){
    scanf("%d", &nrlin1);
    scanf("%d", &nrcol1);

    m1 = (int*) malloc(sizeof(int) * nrlin1);
    for(int i = 0; i < nrlin1; ++i)
        m1[i] = (int*) malloc(sizeof(int)*nrcol1);
    
    for(int i = 0; i < nrlin1; ++i)
        for(int j = 0; j < nrcol1; ++j)
            scanf("%d", &m1[i][j]);

    scanf("%d", &nrlin2);
    scanf("%d", &nrcol2);
    
    m2 = (int*) malloc(sizeof(int) * nrlin2);
    for(int i = 0; i < nrlin2; ++i)
        m2[i] = (int*) malloc(sizeof(int)*nrcol2);
    
    for(int i = 0; i < nrlin2; ++i)
        for(int j = 0; j < nrcol2; ++j)
            scanf("%d", &m2[i][j]);

    for(int i = 0; i < nrlin2; ++i){
        for(int j = 0; j < nrcol2; ++j)
            printf("%d ", m2[i][j]);
        printf("\n");
    }

    if(nrcol1 != nrlin2){
        perror(NULL);
        return errno;
    }
    else{
        pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * nrlin1 * nrcol2);
        int number_threads = 0;
        result = (int**) malloc(sizeof(int) * nrlin1);
        for(int i = 0; i < nrlin1; ++i)
            result[i] = (int*) malloc(sizeof(int)*nrcol2);

        for(int i = 0;i < nrlin1; ++i)
            for(int j = 0; j < nrcol2; ++j){
                struct thread_params index = (struct thread_params) malloc(sizeof(struct thread_params));
                index->line = i;
                index->column = j;      

                if(pthread_create(&threads[number_threads++], NULL,multiply, index)){
                    perror(NULL);
                    return errno;
                }
                
            }

        number_threads = 0;
        for(int i = 0;i < nrlin1; ++i)
            for(int j = 0; j < nrcol2; ++j){
                void *retval;
                
                if(pthread_join(threads[number_threads++], &retval)){
                    perror(NULL);
                    return errno;
                }
                int* ret = (int*) retval;
                result[i][j] = *ret;
                free(ret);
            }
        
        for(int i = 0; i < nrlin1; ++i){
            for(int j = 0; j < nrcol2; ++j)
                printf("%d ", result[i][j]);
            printf("\n");
        }
        for(int i = 0; i < nrlin1; ++i)
            free(result[i]);
        free(result);
        free(threads);
    }
    for(int i = 0;i < nrlin1; ++i)
        free(m1[i]);
    free(m1);

    for(int i = 0; i < nrlin2; ++i)
        free(m2[i]);
    free(m2);

    
    return 0;
}