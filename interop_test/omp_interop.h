//
// Created by Yonghong Yan on 4/29/16.
//
#ifndef __LIBOMP_OMP_INTEROP_H__
#define __LIBOMP_OMP_INTEROP_H__

typedef enum omp_wait_policy_t {
    OMP_ACTIVE_WAIT = 1,
    OMP_PASSIVE_WAIT = 2,
} omp_wait_policy_t;

/* omp_set_wait_policy: to allow users to dynamically control the behavior of the thread during the sequential region. */
void omp_set_wait_policy(omp_wait_policy_t wait_policy);

/* return the current wait policy */
omp_wait_policy_t omp_get_wait_policy(void);

/* The omp_quiesce routine quiesces all OpenMP threads associated with the generating task. Quiescence may involve termination
 * of threads or otherwise inactivating them. The routine returns zero if quiescence has been achieved,
 * otherwise it returns a non-zero error code.
 */
int omp_quiesce(void);

/**
 * This routine allows user to create an OpenMP thread without using parallel, and lets it be a user thread
 * similar to a native thread such as pthread. The thread created does not belong to a team,
 * but is visible to the OpenMP runtime. Ref: pthread_create manual
 *
 * @param new_stack: the stack for the new thread, NULL if use original stack
 *
 * If successful,  the omp_thread_create() function will return zero.
 * Otherwise, an error number will be returned to indicate the error.
 * E.g. The runtime system lacked the necessary resources to create another thread,
 * or the runtime-imposed limit on the total number of threads, e.g.
 * OMP_THREAD_LIMIT would be exceeded.
 */
typedef struct omp_thread omp_thread_t;
int omp_thread_create (omp_thread_t * th, void *(*start_routine)(void *), void *arg, void * new_stack);

/**
 * The omp_thread_exit function terminates the calling OpenMP thread and makes the value value_ptr available to
 * any successful join with the terminating thread. Ref: pthread_exit.
 */
void omp_thread_exit(void *value_ptr);

/**
 * The omp_thread_join function suspends execution of the calling thread until the target thread terminates,
 * unless the target thread has already terminated. Ref: pthread_join.
 * On return from a successful omp_thread_join call with a non-NULL value_ptr argument,
 * the value passed to omp_thread_exit by the terminating thread is stored in the location referenced by value_ptr.
 * When a omp_thread_join returns successfully, the target thread has been terminated.
 * The results of multiple simultaneous calls to omp_thread_join, specifying the same target thread, are undefined.
 */
int omp_thread_join(omp_thread_t thread, void **value_ptr);

/** OpenMP threads managed using omp_thread_create, omp_thread_exit and omp_thread_join are non-team threads.
 * Threadprivate does not apply to those threads. The effect of using omp_thread_exit and omp_thread_join for
 * team threads is undefined.
 */

/**
 * The omp_thread_attach function attaches the calling user thread to an existing OpenMP runtime.
 *
 * new_stack: the stack after attaching, NULL if use original stack.
 * If successful, the omp_thread_attach() function will return zero.
 * Otherwise, an error number will be returned to indicate the error.
 *
 * This function allows users to provide a callback so the thread can return back to user.
 * Each time this thread finishes its OpenMP tasks (e.g. finishing a parallel region), the callback_flag
 * will be evaluated. The flag will also be evaluated while the thread is waiting for more work from OpenMP.
 * When the flag becomes non-zero, the thread will be removed from the OpenMP runtime and return to users starting
 * from the continuation of this call.
 *
 * If callback_flag is NULL, the attached user threads will not return to users until the OpenMP runtime terminates
 *
 * Attaching an OpenMP thread to the OpenMP runtime will be no-ops
 */
int omp_thread_attach(void * new_stack, int * callback_flag);

/**
 * The omp_runtime_handle_t type and omp_get_runtime_handle routines are used for handling the situation in which
 * multiple OpenMP runtime instances exist in a program.
 */
typedef struct omp_runtime_handle omp_runtime_handle_t;
omp_runtime_handle_t omp_get_runtime_handle();

#endif //__LIBOMP_OMP_INTEROP_H__
