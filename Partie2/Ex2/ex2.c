#include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #define VECTOR_LENGTH 400
    #define NB_THREADS	4
    pthread_mutex_t lock;

    int vec1[VECTOR_LENGTH]; //Vector 1
    int vec2[VECTOR_LENGTH]; //Vector 2
    int res;
    int rc,t;
    void *computeDotProduct(void * n)
    {
        int i;
        int local_res;
        int j;
        j=*((int *)n);
        
        //Compute the dot product of 100 elements of vec1 and vec2
        
        for(i=0;i<VECTOR_LENGTH/4;i++){
            local_res+=vec1[i+j*100]*vec2[i+j*100];

        }



        //Update the variable res --> Need of synchronisation here
        pthread_mutex_lock(&lock);
        
        res+=local_res;
        pthread_mutex_unlock(&lock);
    }
    
    int main (int argc, char *argv[])
    {
        pthread_t thread[NB_THREADS];
        pthread_attr_t attr;
        pthread_mutex_init(&lock,NULL);
        //Initialise vec1 and vec2
        int tabind[NB_THREADS] ={0,1,2,3};

        for(t=0;t<VECTOR_LENGTH;t++){
            vec1[t] = t;
            vec2[t] =t; 
        }        

        //Create 4 threads to compute the result
         for(t=0; t<NB_THREADS; t++)
            {
                printf("Creation du thread %d\n", t);
                rc = pthread_create(&thread[t], &attr, computeDotProduct, (void *)(&tabind[t])); 
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
        printf("le produit scalaire par thread est :\t %d\n",res);

    
        pthread_mutex_destroy(&lock);
        
    }