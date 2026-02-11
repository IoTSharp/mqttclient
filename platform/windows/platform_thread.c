/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-23 19:26:27
 * @LastEditTime: 2020-02-23 16:19:07
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "platform_thread.h"
#include "platform_memory.h"

typedef struct {
    void (*entry)(void *);
    void *param;
    HANDLE start_event;
    HANDLE stop_event;
} thread_wrapper_param_t;

static DWORD WINAPI thread_wrapper(LPVOID arg)
{
    thread_wrapper_param_t *wrapper_param = (thread_wrapper_param_t *)arg;
    
    // Wait for start signal
    WaitForSingleObject(wrapper_param->start_event, INFINITE);
    
    // Call the actual thread function
    wrapper_param->entry(wrapper_param->param);
    
    return 0;
}

platform_thread_t *platform_thread_init( const char *name,
                                        void (*entry)(void *),
                                        void * const param,
                                        unsigned int stack_size,
                                        unsigned int priority,
                                        unsigned int tick)
{
    platform_thread_t *thread;
    thread_wrapper_param_t *wrapper_param;
    DWORD thread_id;
    
    (void)name;
    (void)priority;
    (void)tick;
    
    thread = (platform_thread_t *)platform_memory_alloc(sizeof(platform_thread_t));
    if (thread == NULL) {
        return NULL;
    }
    
    wrapper_param = (thread_wrapper_param_t *)platform_memory_alloc(sizeof(thread_wrapper_param_t));
    if (wrapper_param == NULL) {
        platform_memory_free(thread);
        return NULL;
    }
    
    // Create events for thread synchronization
    thread->start_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    thread->stop_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    
    if (thread->start_event == NULL || thread->stop_event == NULL) {
        if (thread->start_event) CloseHandle(thread->start_event);
        if (thread->stop_event) CloseHandle(thread->stop_event);
        platform_memory_free(wrapper_param);
        platform_memory_free(thread);
        return NULL;
    }
    
    wrapper_param->entry = entry;
    wrapper_param->param = param;
    wrapper_param->start_event = thread->start_event;
    wrapper_param->stop_event = thread->stop_event;
    
    // Create the thread
    thread->thread = CreateThread(
        NULL,
        stack_size,
        thread_wrapper,
        wrapper_param,
        0,
        &thread_id
    );
    
    if (thread->thread == NULL) {
        CloseHandle(thread->start_event);
        CloseHandle(thread->stop_event);
        platform_memory_free(wrapper_param);
        platform_memory_free(thread);
        return NULL;
    }
    
    return thread;
}

void platform_thread_startup(platform_thread_t* thread)
{
    (void) thread;
}

void platform_thread_stop(platform_thread_t* thread)
{
    if (thread != NULL) {
        WaitForSingleObject(thread->stop_event, INFINITE);
    }
}

void platform_thread_start(platform_thread_t* thread)
{
    if (thread != NULL) {
        SetEvent(thread->start_event);
    }
}

void platform_thread_destroy(platform_thread_t* thread)
{
    if (thread != NULL) {
        if (thread->thread != NULL) {
            CloseHandle(thread->thread);
        }
        if (thread->start_event != NULL) {
            CloseHandle(thread->start_event);
        }
        if (thread->stop_event != NULL) {
            CloseHandle(thread->stop_event);
        }
        platform_memory_free(thread);
    }
}
