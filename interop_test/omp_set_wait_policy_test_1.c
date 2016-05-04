#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <sys/timeb.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

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

int main(int argc, char * argv[]) {
    int nthreads = 4;
    if (argc >= 2) nthreads = (atoi(argv[1]));
#pragma omp parallel num_threads(nthreads)
    {
        int tid = omp_get_thread_num();
    }

    omp_set_wait_policy(OMP_ACTIVE_WAIT);

    if (omp_get_wait_policy() == OMP_ACTIVE_WAIT) {
        printf("WAIT policy is not correctly set\n");
    }
#pragma omp parallel num_threads(nthreads)
    {
        int tid = omp_get_thread_num();
    }

    omp_set_wait_policy(OMP_PASSIVE_WAIT);

    if (omp_get_wait_policy() == OMP_PASSIVE_WAIT) {
        printf("WAIT policy is not correctly set\n");
    }

#pragma omp parallel num_threads(nthreads)
    {
        omp_set_wait_policy(OMP_PASSIVE_WAIT); /* This is not the "recommended" way to use */
        int tid = omp_get_thread_num();
    }
    return 0;
}

