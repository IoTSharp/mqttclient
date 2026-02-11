#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_
/* Platform-specific includes */
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#define sleep(x) Sleep((x)*1000)  /* Windows Sleep is in milliseconds */
typedef HANDLE pthread_t;
typedef DWORD(WINAPI* thread_func_t)(LPVOID);
#define pthread_create(thread, attr, start_routine, arg) \
        ((*thread = CreateThread(NULL, 0, (thread_func_t)(start_routine), arg, 0, NULL)) == NULL ? -1 : 0)
#define     SALOF_OS                            SALOF_USING_WINDOWS
#else
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#define     SALOF_OS                            SALOF_USING_LINUX
#endif
#endif