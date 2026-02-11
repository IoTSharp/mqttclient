/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2019-12-10 22:16:41
 * @LastEditTime: 2020-06-05 17:18:48
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#include "platform_timer.h"

void platform_timer_init(platform_timer_t* timer)
{
    QueryPerformanceFrequency(&timer->frequency);
    timer->end_time.QuadPart = 0;
}

void platform_timer_cutdown(platform_timer_t* timer, unsigned int timeout)
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    
    // Calculate end time in performance counter units
    // timeout is in milliseconds
    LONGLONG timeout_ticks = (LONGLONG)timeout * timer->frequency.QuadPart / 1000;
    timer->end_time.QuadPart = now.QuadPart + timeout_ticks;
}

char platform_timer_is_expired(platform_timer_t* timer)
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (now.QuadPart >= timer->end_time.QuadPart) ? 1 : 0;
}

int platform_timer_remain(platform_timer_t* timer)
{
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    
    if (now.QuadPart >= timer->end_time.QuadPart) {
        return 0;
    }
    
    // Calculate remaining time in milliseconds
    LONGLONG remain_ticks = timer->end_time.QuadPart - now.QuadPart;
    return (int)(remain_ticks * 1000 / timer->frequency.QuadPart);
}

unsigned long platform_timer_now(void)
{
    return (unsigned long)time(NULL);
}

void platform_timer_usleep(unsigned long usec)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, now;
    LONGLONG target_ticks;
    
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    
    // Convert microseconds to performance counter ticks
    target_ticks = (LONGLONG)usec * frequency.QuadPart / 1000000;
    
    // For very short sleeps, use busy wait for accuracy
    if (usec < 100) {
        do {
            QueryPerformanceCounter(&now);
        } while ((now.QuadPart - start.QuadPart) < target_ticks);
    } else {
        // For longer sleeps, use Sleep to avoid CPU spinning
        // Sleep for most of the duration, then busy wait for the remainder
        DWORD sleep_ms = (DWORD)(usec / 1000);
        if (sleep_ms > 1) {
            Sleep(sleep_ms - 1);
        }
        
        // Busy wait for the remainder
        do {
            QueryPerformanceCounter(&now);
        } while ((now.QuadPart - start.QuadPart) < target_ticks);
    }
}
