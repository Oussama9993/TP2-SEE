#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NB_THREADS	3

void *travailUtile(void *null)
{
    int i;
    double resultat=0.0;
    for (i=0; i<1000000; i++)
    {
        resultat = resultat + (double)random();
    }
    printf("resultat = %e\n",resultat);
    pthread_exit((void *) 0);
}

int main (int argc, char *argv[])
{
    pthread_t thread[NB_THREADS];
    pthread_attr_t attr;
    size_t size;
    int rc, t;
    void *status;

    /* Initialisation et activation d’attributs */
    pthread_attr_init(&attr); //valeur par défaut
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); //attente du thread possible
    size=1048576;
    printf("succès de setstack ? %d\n",pthread_attr_setstacksize(&attr, size));
    for(t=0; t<NB_THREADS; t++)
    {
        printf("Creation du thread %d\n", t);
        rc = pthread_create(&thread[t], &attr, travailUtile, NULL); 
        if (rc)
        {
            printf("ERROR; le code de retour de pthread_create() est %d\n", rc);
            exit(-1);
        }
    }
    
    printf("succès ?: %d\n",pthread_attr_getstacksize(&attr,&size));
    printf("stack size: %d\n",(int)size);
    /* liberation des attributs et attente de la terminaison des threads */
    pthread_attr_destroy(&attr);
    for(t=0; t<NB_THREADS; t++)
    {
        rc = pthread_join(thread[t], &status);
        if (rc)
        {
            printf("ERROR; le code de retour du pthread_join() est %d\n", rc);
            exit(-1);
    }
    printf("le join a fini avec le thread %d et a donne le status= %ld\n",t, (long)status);
    }

    pthread_exit(NULL);
}