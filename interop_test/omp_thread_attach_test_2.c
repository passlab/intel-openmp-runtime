// execute in sequence
// input the number of num_user_threadsation

#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <sys/timeb.h>
#include <omp_interop.h>
#include <unistd.h>

/* read timer in second */
double read_timer() {
    struct timeb tm;
    ftime(&tm);
    return (double) tm.time + (double) tm.millitm / 1000.0;
}

int user_thread_return = 0;

void *test_fun(void *arg){
    printf("thread: %d\n", *((int*)arg));
    omp_thread_attach_with_callback(NULL, &user_thread_return);
    printf("thread call back: %d\n", *((int*)arg));

    return ((void*)0);
}

int main(int argc, char * argv[]) {
     if (argc >= 2){
        omp_set_num_threads(atoi(argv[1]));
        num_user_threads = atoi(argv[1]);
    }

    int num_user_threads = 100;
    pthread_t pthreads[num_user_threads];

    // pthread_create
    for(i=0; i<num_user_threads; i++){
        pthread_create(&pthreads[i], NULL, test_fun, NULL);
    }

    // omp_set_nested();
    // create 50 threads and put them into threadpool
#pragma omp parallel shared(user_thread_id, counter) private(tid) num_threads(50)
    {
        tid = omp_get_thread_num();
    }

    user_thread_return = 1;

    for(i=0; i<num_user_threads; i++) {
        pthread_join(pthreads[i], NULL);
    }

    // while(1);
}


