#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

#define VECTOR_LENGTH 400
#define NB_THREADS	4
    pthread_mutex_t lock;
    

    //int vec1[VECTOR_LENGTH]; //Vector 1
    //int vec2[VECTOR_LENGTH]; //Vector 2
    //int res;
    int rc,t;
    typedef struct 
    {
        int vec1[VECTOR_LENGTH];
        int vec2[VECTOR_LENGTH];
        int res;
        int counter;
        int local_res[NB_THREADS];

        
    }shm_infos;
    shm_infos *shm_inst;
    
    
    void *computeDotProduct(void * n)
    {
        int i;
        int j;
        j=*((int *)n);
        
        //Compute the dot product of 100 elements of vec1 and vec2
        
        for(i=0;i<VECTOR_LENGTH/4;i++){
            shm_inst->local_res[j]+=shm_inst->vec1[i+j*100]*shm_inst->vec2[i+j*100];

        }



        //Update the variable res --> Need of synchronisation here
        pthread_mutex_lock(&lock);
        
        shm_inst->res+=shm_inst->local_res[j];
        pthread_mutex_unlock(&lock);
    }
    
    int main (int argc, char *argv[])
    {
        pthread_t thread[NB_THREADS];
        pthread_attr_t attr;
        pthread_mutex_init(&lock,NULL);
        const char name[]="/Block_mem";
        int fd;
        fd=shm_open(name,O_CREAT| O_RDWR,0666);
        ftruncate(fd, sizeof(shm_infos));
        shm_inst=(shm_infos *)mmap(NULL,sizeof(shm_infos),PROT_WRITE,MAP_SHARED,fd,0);

        //Initialise vec1 and vec2
        int tabind[NB_THREADS] ={0,1,2,3};

        for(t=0;t<VECTOR_LENGTH;t++){
            shm_inst->vec1[t] = t;
            shm_inst->vec2[t] =VECTOR_LENGTH-1-t; 
        }        

        //Create 4 threads to compute the result
         for(t=0; t<NB_THREADS; t++)
            {
                printf("Creation du thread %d\n", t);
                rc = pthread_create(&thread[t], NULL, computeDotProduct, (void *)(&tabind[t])); 
                if (rc)
                {
                    printf("ERROR; le code de retour de pthread_create() est %d\n", rc);
                    exit(-1);
                }
            }

        //Remove threads
        for(t=0; t<NB_THREADS; t++)
            {
                rc = pthread_join(thread[t], NULL);
                if (rc)
                {
                    printf("ERROR; le code de retour du pthread_join() est %d\n", rc);
                    exit(-1);
                }
                printf("le join a fini avec le thread %d \n",t);
            }
        printf("le produit scalaire par thread est :\t %d \n",shm_inst->res);

    
        pthread_mutex_destroy(&lock);
        munmap(shm_inst,sizeof(shm_infos));
        close(fd);
        shm_unlink(name);
        
    }