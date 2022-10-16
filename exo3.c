#include <pthread.h>
#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#define MSG_SIZE 4096
#define VECTOR_LENGTH 400
#define NB_THREADS	4
    pthread_mutex_t lock;
    mqd_t mq;
    

    int vec1[VECTOR_LENGTH]; //Vector 1
    int vec2[VECTOR_LENGTH]; //Vector 2
    int res;
    int rc,t;
    void handler (int sig_num) {
        printf ("sig %d recu.\n", sig_num);
        }
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
        mq=mq_open("/mymq",O_WRONLY | O_CREAT,0600,NULL);



        //Update the variable res --> Need of synchronisation here
        pthread_mutex_lock(&lock);
        
        res+=local_res;
        while (1){
            mq_send(mq,(char *)&local_res,sizeof(int),1);
            sleep(1);
        }
        pthread_mutex_unlock(&lock);
    }
    
    int main (int argc, char *argv[])
    {

        pthread_t thread[NB_THREADS];
        pthread_attr_t attr1;
        /* Section atribut de la la file de" message*/
        struct mq_attr attr , mq_attr_old;
        int taille;
        char *buffer;
        struct sigevent sigevent;
        
        mq=mq_open("/mymq",O_RDONLY | O_CREAT,0600,NULL);
        if (mq == (mqd_t) -1) {
            perror(argv[1]); //prints an error message to stderr
            exit(EXIT_FAILURE);
        }
        if (mq_getattr(mq,&attr)!=0){
            perror("mq_getattr");
            exit(EXIT_FAILURE);
        }
        taille = attr.mq_msgsize;
        buffer = malloc(taille);
        if (buffer == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
            }         
        
        char buf[MSG_SIZE];
        unsigned int prio;
        pthread_mutex_init(&lock,NULL);
        //Initialise vec1 and vec2
        int tabind[NB_THREADS] ={0,1,2,3};

        for(t=0;t<VECTOR_LENGTH;t++){
            vec1[t] = t;
            vec2[t] =VECTOR_LENGTH-1-t; 
        }        

        //Create 4 threads to compute the result
         for(t=0; t<NB_THREADS; t++)
            {
                printf("Creation du thread %d\n", t);
                rc = pthread_create(&thread[t], &attr1, computeDotProduct, (void *)(&tabind[t])); 
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
        printf("le produit scalaire par thread est :\t %d",res);

    
        pthread_mutex_destroy(&lock);
        
    }