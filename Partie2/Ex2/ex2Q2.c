#include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #define VECTOR_LENGTH 400
    #define NB_THREADS	5
    pthread_mutex_t lock;
    pthread_cond_t cond;

    int vec1[VECTOR_LENGTH]; //Vector 1
    int vec2[VECTOR_LENGTH]; //Vector 2
    int res=0;
    int rc,t;
    int counter=0;
    void *computeDotProduct(void * n)
    {
        int i;
        int local_res;
        int j;
        j=*((int *)n);
        
        //Compute the dot product of 100 elements of vec1 and vec2

        
            for(i=0;i<VECTOR_LENGTH/(NB_THREADS-1);i++){
            local_res+=vec1[i+j*100]*vec2[i+j*100];

            }
        //Update the variable res --> Need of synchronisation here
        pthread_mutex_lock(&lock);
        res+=local_res;
        counter++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }
    void *counter_t(void *n){
        pthread_mutex_lock(&lock);
        while(counter!=4){
            pthread_cond_wait(&cond,&lock);

        }
        printf("le résultat global par le thread %d est %d \n",*((int *)n),res);

        pthread_mutex_unlock(&lock);
    }
    
    int main (int argc, char *argv[])
    {
        pthread_t thread[NB_THREADS];
        pthread_attr_t attr;
        pthread_cond_init(&cond,NULL);
        pthread_mutex_init(&lock,NULL); 
        pthread_attr_init(&attr);       
        //Initialise vec1 and vec2
        int tabind[NB_THREADS] ={0,1,2,3,4};

        for(t=0;t<VECTOR_LENGTH;t++){
            vec1[t] = t;
            vec2[t] =VECTOR_LENGTH-1-t; 
        }        

        //Create 4 threads to compute the result
         for(t=0; t<NB_THREADS; t++)
            {
                printf("Creation du thread %d\n", t);
                if (t==4){
                    rc = pthread_create(&thread[t], &attr, counter_t, (void *)(&tabind[t])); 

                }
                else{
                    rc = pthread_create(&thread[t], &attr, computeDotProduct, (void *)(&tabind[t])); 
                    if (rc)
                {
                    printf("ERROR; le code de retour de pthread_create() est %d\n", rc);
                    exit(-1);
                }
                }
                if (rc)
                {
                    printf("ERROR; le code de retour de pthread_create() est %d\n", rc);
                    exit(-1);
                }


            }
        //Create the display thread
        


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
        pthread_cond_destroy(&cond);
        pthread_attr_destroy(&attr);
        
        printf("le produit scalaire par thread est :\t %d",res);

    
        pthread_mutex_destroy(&lock);
        
    }