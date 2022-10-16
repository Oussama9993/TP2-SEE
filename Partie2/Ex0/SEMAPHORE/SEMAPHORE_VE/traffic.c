#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

pthread_t tid[2];
sem_t semaphore;

void *road_horizontal(void *nb_car)
{
    struct timespec period;
    int nb = *(int *)nb_car;
    clock_gettime(CLOCK_REALTIME, &period);

    for (int i = 0; i < nb; i++)
    {   
        sem_wait(&semaphore); 
        printf("T=%d : road horizontal: Car %d passes\n", i*3, i);
        //Simple task to simulate car passing by the intersection
        for (int j = 0; j < 1000000000; j++);
        sem_post(&semaphore);
        //Debit: 1 car every 3 seconds  
            
        period.tv_sec += 3;
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &period, NULL);
        
    }    

    return NULL;
}

void *road_vertical(void *nb_car)
{
    int nb = *(int *)nb_car;
    struct timespec period;
    clock_gettime(CLOCK_REALTIME, &period);

    for (int i = 0; i < nb; i++)
    {
        sem_wait(&semaphore);
        printf("T=%d : road vertical: Car %d passes\n", i*2, i);
        //Simple task to simulate car passing by the intersection
        for (int j = 0; j < 1000000000; j++);

        //Debit: 1 car every 2 seconds
        sem_post(&semaphore);
        period.tv_sec += 2;
        clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &period, NULL);
         

    }
    return NULL;
}

int main(int argc, char **argv)
{
    int nb_car = 10;

    sem_init(&semaphore, PTHREAD_PROCESS_SHARED, 1);

    pthread_create(&(tid[0]), NULL, &road_horizontal, &nb_car);
    pthread_create(&(tid[1]), NULL, &road_vertical, &nb_car);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    sem_destroy(&semaphore);

    return 0;
}