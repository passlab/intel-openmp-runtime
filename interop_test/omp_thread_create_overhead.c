// execute in sequence
// input the number of num_user_threadsation

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <omp.h>
#include <sys/timeb.h>
//#include <omp_interop.h>
#include <unistd.h>

/* read timer in second */
double read_timer() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time + (double) tm.millitm / 1000.0;
}

void *test_fun(void *arg){
    printf("miccc2 ********** %d\n", *((int*)arg));
    return ((void*)0);
}

int main(int argc, char * argv[]) {
   int num_user_threads;
   int i, tid;
     if (argc >= 2){
        omp_set_num_threads(atoi(argv[1]));
        num_user_threads = atoi(argv[1]);
    }

    // omp_set_nested();
    // create 50 threads and put them into threadpool
    #pragma omp parallel private(tid) num_threads(50)
    {
        tid = omp_get_thread_num(); 
    }

    num_user_threads = 100;
    omp_thread_t omp_threads[num_user_threads];
    pthread_t pthreads[num_user_threads];

    // omp_thread_create
    double tt = read_timer();
    for(i=0; i<num_user_threads; i++){
        omp_thread_create(&omp_threads[i], test_fun, &i, NULL);
    }
    for(i=0; i<num_user_threads; i++){
        omp_thread_join(&omp_threads[i], NULL);
    }
    tt = read_timer()-tt;
    printf("omp thread execution time:%f\n", tt);
   
    // pthread_create
    tt = read_timer();
    for(i=0; i<num_user_threads; i++){
        pthread_create(&pthreads[i], NULL, test_fun, &i);
    }
    for(i=0; i<num_user_threads; i++) {
        pthread_join(pthreads[i], NULL);
    }
    tt = read_timer()-tt;
    printf("pthread execution time:%f\n", tt);

    // while(1);
}


