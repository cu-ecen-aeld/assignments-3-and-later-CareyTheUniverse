#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    
    DEBUG_LOG("Waiting to obtain mutex");
    usleep(thread_func_args->wait_to_obtain_ms * 1000);
    
    int rc = pthread_mutex_lock(thread_func_args->mutex);
    if (rc != 0) 
    {
        ERROR_LOG("Failed to obtain mutex: %d", rc);
        return false;
    }
    else
    {
        DEBUG_LOG("Mutex obtained");
    }

    DEBUG_LOG("Waiting before releasing mutex");
    usleep(thread_func_args->wait_to_release_ms * 1000);

    rc = pthread_mutex_unlock(thread_func_args->mutex);
    if (rc != 0) 
    {
        ERROR_LOG("Failed to release mutex: %d", rc);
        return false;
    }
    else
    {
        DEBUG_LOG("Mutex released");
    }
    
    thread_func_args->thread_complete_success = true;
    
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
     
    struct thread_data* data = (struct thread_data *) malloc(sizeof(struct thread_data));
    
    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;
    
    int ptc = pthread_create(thread, NULL, threadfunc, data);
    if (ptc == 0) 
    {
       DEBUG_LOG("Allocated memory for thread data");
       return true;
    }
    else
    {
       ERROR_LOG("Error: Could not allocate memory for thread data");
       return false;
    }
}

