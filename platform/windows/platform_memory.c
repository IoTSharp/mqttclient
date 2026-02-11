/*
 * @Author: jiejie
 * @Github: https://github.com/IoTSharp
 * @Date: 2026-02-11 00:00:00
 * @LastEditTime: 2026-02-11 03:20:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "platform_memory.h"

void *platform_memory_alloc(size_t size)
{
    return malloc(size);
}

void *platform_memory_calloc(size_t num, size_t size)
{
    return calloc(num, size);
}

void platform_memory_free(void *ptr)
{
    free(ptr);
}
