/*
 * @Author: IoTSharp
 * @Github: https://github.com/IoTSharp
 * @Date: 2026-02-11
 * @Description: Windows platform implementation for salof logging
 */
#include "salof_defconfig.h"

#ifdef SALOF_USING_LOG

#include <time.h>

void *salof_alloc(unsigned int size)
{
    return malloc((size_t)size);
}

void salof_free(void *mem)
{
    free(mem);
}

salof_tcb salof_task_create(const char *name,
                            void (*task_entry)(void *param),
                            void * const param,
                            unsigned int stack_size,
                            unsigned int priority,
                            unsigned int tick)
{
    salof_tcb task;
    DWORD thread_id;
    
    (void)name;
    (void)priority;
    (void)tick;
    
    task = salof_alloc(sizeof(HANDLE));
    if (NULL != task) {
        *task = CreateThread(
            NULL,
            stack_size,
            (LPTHREAD_START_ROUTINE)task_entry,
            param,
            0,
            &thread_id
        );
        
        if (*task == NULL) {
            salof_free(task);
            task = NULL;
        }
    }
    
    return task;
}

salof_mutex salof_mutex_create(void)
{
    salof_mutex mutex;
    mutex = salof_alloc(sizeof(CRITICAL_SECTION));
    
    if (NULL != mutex)
        InitializeCriticalSection(mutex);
    
    return mutex;
}

void salof_mutex_delete(salof_mutex mutex)
{
    if (NULL != mutex) {
        DeleteCriticalSection(mutex);
        salof_free(mutex);
    }
}

int salof_mutex_pend(salof_mutex mutex, unsigned int timeout)
{
    if (NULL == mutex)
        return -1;
        
    if (timeout == 0) {
        return TryEnterCriticalSection(mutex) ? 0 : -1;
    }
    
    EnterCriticalSection(mutex);
    return 0;
}

int salof_mutex_post(salof_mutex mutex)
{
    if (NULL == mutex)
        return -1;
        
    LeaveCriticalSection(mutex);
    return 0;
}

salof_sem salof_sem_create(void)
{
    salof_sem sem;
    sem = salof_alloc(sizeof(HANDLE));
    
    if (NULL != sem) {
        *sem = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
        if (*sem == NULL) {
            salof_free(sem);
            sem = NULL;
        }
    }
    
    return sem;
}

void salof_sem_delete(salof_sem sem)
{
    if (NULL != sem) {
        if (*sem != NULL)
            CloseHandle(*sem);
        salof_free(sem);
    }
}

int salof_sem_pend(salof_sem sem, unsigned int timeout)
{
    DWORD result;
    
    if (NULL == sem || *sem == NULL)
        return -1;
    
    result = WaitForSingleObject(*sem, (timeout == 0) ? INFINITE : timeout);
    return (result == WAIT_OBJECT_0) ? 0 : -1;
}

int salof_sem_post(salof_sem sem)
{
    if (NULL == sem || *sem == NULL)
        return -1;
        
    return ReleaseSemaphore(*sem, 1, NULL) ? 0 : -1;
}

unsigned int salof_get_tick(void)
{
    return (unsigned int)time(NULL);
}

char *salof_get_task_name(void)
{
    return NULL;
}

int send_buff(char *buf, int len)
{
    fputs(buf, stdout);
    fflush(stdout);
    return len;
}

#endif
