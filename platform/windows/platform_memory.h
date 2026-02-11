/*
 * @Author: jiejie
 * @Github: https://github.com/IoTSharp
 * @Date: 2026-02-11 00:00:00
 * @LastEditTime: 2026-02-11 03:20:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _PLATFORM_MEMORY_H_
#define _PLATFORM_MEMORY_H_
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void *platform_memory_alloc(size_t size);
void *platform_memory_calloc(size_t num, size_t size);
void platform_memory_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif
