/*
 * @Author: jiejie
 * @Github: https://github.com/IoTSharp
 * @Date: 2026-02-11 00:00:00
 * @LastEditTime: 2026-02-11 03:20:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _PLATFORM_MUTEX_H_
#define _PLATFORM_MUTEX_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct platform_mutex {
    CRITICAL_SECTION mutex;
} platform_mutex_t;

int platform_mutex_init(platform_mutex_t* m);
int platform_mutex_lock(platform_mutex_t* m);
int platform_mutex_trylock(platform_mutex_t* m);
int platform_mutex_unlock(platform_mutex_t* m);
int platform_mutex_destroy(platform_mutex_t* m);

#ifdef __cplusplus
}
#endif

#endif
