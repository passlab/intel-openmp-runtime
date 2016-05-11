// execute in parallel
// input the number of thread 

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

void *test_fun(void *arg){
    printf("omp_thread: %d\n", *((int*)arg));
    return arg:
}

void *test_func_2(void *arg) {
    printf("omp_thread: %d\n", *((int*)arg));
    omp_thread_exit(arg);
}

int num_threads = 100;
int iter = 10000;

int main(int argc, char * argv[])
{
     if (argc >= 2){
        omp_set_num_threads(atoi(argv[1]));
        num_threads = atoi(argv[1]);
    }

    // omp_set_nested();
    // create 50 threads and put them into threadpool
    #pragma omp parallel shared(user_thread_id, counter) private(tid) num_threads(50)
    {
        tid = omp_get_thread_num(); 
    }
    int ret_value;

    omp_thread_t ompthread_0;
    omp_thread_create(&ompthread_0, test_fun, (void*)0, NULL);

    omp_thread_join(&ompthread_0, &ret_value);
    printf("omp_thread 0 return: %d\n", ret_value);

    omp_thread_t ompthread_1;
    void * stack = malloc(4098);
    omp_thread_create(&ompthread_1, test_fun, (void*)1, stack);
    omp_thread_join(&ompthread_1, &ret_value);
    printf("omp_thread 1 return: %d\n", ret_value);

    omp_thread_t ompthread_2;
    void * stack = malloc(4098);
    omp_thread_create(&ompthread_2, test_fun, (void*)2, stack);
    omp_thread_join(&ompthread_2, &ret_value);
    printf("omp_thread 2 return: %d\n", ret_value);

    return 0;
}


