// input number of thread

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <omp.h>
//#include <omp_interop.h>
#include <sys/timeb.h>
#include <unistd.h>
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

 double read_timer() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time + (double) tm.millitm / 1000.0;
}
void omp_quiesce_overhead(int nthreads);

int main(int argc, char * argv[]) {
	int thr_num = 4;
	if (argc >= 2) thr_num = (atoi(argv[1]));
    omp_quiesce_overhead(thr_num);

    exit(0);
}

/**
 * TODO: how to make sure that an empty parallel do not get optimized out by the compiler
 */
void omp_quiesce_overhead(int nthreads) {
    int i;
    int NUM_ITERATIONS = 1000;
    double quiesce_ov = 0.0;
    double quiesce_start_ov = 0.0;
    
    double cost_all = read_timer();
    for (i=0; i<NUM_ITERATIONS; i++) {
        //double temp = read_timer();
		#pragma omp parallel num_threads(nthreads)
		{
			//int tid = omp_get_thread_num();
		}
		
		double temp2  = read_timer();
		omp_quiesce();
		quiesce_ov += read_timer() - temp2;
        //quiesce_start_ov += read_timer() - temp;
    }
    cost_all = read_timer() - cost_all;

 	// this is for not quiesce
    double parallel_overhead = read_timer();
    for (i=0; i<NUM_ITERATIONS; i++) {
		#pragma omp parallel num_threads(nthreads)
		{
    		//int tid = omp_get_thread_num();
		}
    }
    parallel_overhead = read_timer() - parallel_overhead;
    printf("quiesce overhead      : %f\n", quiesce_ov/NUM_ITERATIONS);
    printf("quiesce_start overhead: %f\n", (cost_all - parallel_overhead)/NUM_ITERATIONS);
    printf("Total cost:             %f\n", cost_all/NUM_ITERATIONS);

	// while(1);
     return;
}
