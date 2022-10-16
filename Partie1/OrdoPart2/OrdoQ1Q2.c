#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc , char * argv[])
{   
    //Lecture PID du processus courant
    pid_t pid = getpid();
    printf("PID:\t%d\n",pid);
    printf("Priority :\t%d \n",getpriority(PRIO_PROCESS,pid));
    int scheduler =sched_getscheduler(pid);
    if(scheduler==SCHED_OTHER){
        printf("SCHED_OTHER \n");
    }
    else if(scheduler==SCHED_FIFO){
        printf("SCHED_FIFO  \n");
    }
    else if(scheduler==SCHED_RR){
        printf("SCHED_RR \n");
    }
    printf("Policy :\t%d \n",sched_getscheduler(pid));
    int policy=sched_getscheduler(pid);
    printf(" Min priority :\t%d \n",sched_get_priority_min(policy));
    printf(" Max priority :\t%d \n",sched_get_priority_max(policy));
    struct sched_param param;
    printf(" Paramètre :\t%d \n",sched_getparam(pid,&param));
    struct timespec tp;
    printf(" Interval :\t%d \n",sched_rr_get_interval(pid,&tp));
    int newPrio1=setpriority(PRIO_PROCESS,pid,SCHED_FIFO);
    printf("New priority: \t%d \n",getpriority(newPrio1,pid));
    int newPrio2= nice(-5);
    printf("with nice: \t%d \n",newPrio2);
    //Only root user can set the nice value from -20 to 19.
    // Other users can only set nice values from 0 to 19.


}