// 2-process
// set wait policy, test2(0):passive / test2(1):active
// uncomment omp_quiesce() and omp_begin2() to check the time for quiesce policy

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <sys/timeb.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void *omp_parallel_foo(void *ptr);

/**Important: make sure you use num_threads clause in parallel direction and set it to the 
 * number of hardware cores, not the number of cores Linux gives or the default from OpenMP
 * 
 * cat /proc/cpuinfo and check the processor id, core id and CPU model number so you can look up fron internet
 * Lennon is Xeon CPU E5-2683 v3 @ 2.00GHz, it has two CPU for total 28 cores, but support upto 56 threads
 * Paul is Xeon CPU E5-2695 v2 @ 2.40GHz, it has two CPU for total 24 cores, support upto 48 threads
 * Fornax Intel® Xeon® E5-2699 v3 2.3GHz, it has two CPU for total 36 cores, support upto 72 threads.
 *
 * Use -O0 optimization
 */

int total_cores = 2;
void busy_waiting4(int time);
double read_timer() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time + (double) tm.millitm / 1000.0;
}

void test2(int type){
    pid_t proc_id;
    int i = 0;

    double tt;
  
    proc_id = fork();

    if(type==1){
        omp_set_wait_policy(1);  // active 
    }else{
        omp_set_wait_policy(0);  // passive 
    }
    
    if(proc_id==0){
        tt = read_timer();
    }

    omp_set_nested(1);

    for (i=0; i<3; i++){
        if ((proc_id+i)%2==0){
            printf("%d pid %d\n", proc_id, getpid());
            busy_waiting4(1);

            #pragma omp parallel num_threads(2)
            {
                int tid = omp_get_thread_num();
                // printf("level 2 id %d\n", tid);
                busy_waiting4(1);
                int pid = getpid();
                int thid = syscall(SYS_gettid);
                printf("level 2 ompid:%d pid:%d tid:%d\n", tid, pid, thid);

            }
            // omp_quiesce();
            // omp_begin2();
        }else{
            printf("%d pid %d\n", proc_id, getpid());
            int tid = omp_get_thread_num();
            // printf("level 1 id %d\n", tid);
            
            #pragma omp parallel num_threads(2)
            {
                int tid = omp_get_thread_num();
                busy_waiting4(1);
                int pid = getpid();
                int thid = syscall(SYS_gettid);
                printf("level 2 ompid:%d pid:%d tid:%d\n", tid, pid, thid);
            }
            // omp_quiesce();
            // omp_begin2();
            busy_waiting4(1);
        }
    }

    if( proc_id == 0){
        tt = read_timer()-tt;    
        if(type==0){
            printf("passive: %f\n", tt);
        }else{
            printf("active: %f\n", tt);
        }
    }

    // while(1);

}


int main(int argc, char * argv[])
{
	
    // test2(0);   
    test2(1);
}


void busy_waiting4(int second) {
    clock_t ticks1, ticks2;

    ticks1 = clock();
    ticks2 = ticks1;
    while((ticks2/CLOCKS_PER_SEC-ticks1/CLOCKS_PER_SEC) < second)
        ticks2 = clock();

    // printf("It took %ld ticks to wait one second.\n",ticks2-ticks1);
    // printf("This value should be the same as CLOCKS_PER_SEC which is %d.\n",CLOCKS_PER_SEC);
}
