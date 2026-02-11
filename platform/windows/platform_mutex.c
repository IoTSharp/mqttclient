/*
 * @Author: jiejie
 * @Github: https://github.com/IoTSharp
 * @Date: 2026-02-11 00:00:00
 * @LastEditTime: 2026-02-11 03:20:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "platform_mutex.h"

int platform_mutex_init(platform_mutex_t* m)
{
    InitializeCriticalSection(&(m->mutex));
    return 0;
}

int platform_mutex_lock(platform_mutex_t* m)
{
    EnterCriticalSection(&(m->mutex));
    return 0;
}

int platform_mutex_trylock(platform_mutex_t* m)
{
    return TryEnterCriticalSection(&(m->mutex)) ? 0 : -1;
}

int platform_mutex_unlock(platform_mutex_t* m)
{
    LeaveCriticalSection(&(m->mutex));
    return 0;
}

int platform_mutex_destroy(platform_mutex_t* m)
{
    DeleteCriticalSection(&(m->mutex));
    return 0;
}
