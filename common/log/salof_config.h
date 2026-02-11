/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-02-25 06:01:24
 * @LastEditTime: 2020-02-25 09:28:09
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */

#ifndef _SALOF_CONFIG_H_
#define _SALOF_CONFIG_H_

/* 
 * Platform constants MUST be defined FIRST before any code uses them
 * 平台常量必须首先定义，在任何代码使用它们之前
 */
#define         SALOF_USING_RTT             1
#define         SALOF_USING_FREERTOS        2
#define         SALOF_USING_TENCENTOS       3
#define         SALOF_USING_LINUX           4
#define         SALOF_USING_WINDOWS         5

/*
 * 编译时自动平台检测 (Automatic platform detection at compile time)
 * 
 * 本文件会在编译时自动检测操作系统平台，无需手动配置 SALOF_OS
 * This file automatically detects the OS platform at compile time without manual SALOF_OS configuration
 * 
 * 支持的平台 (Supported platforms):
 * - Windows (通过 _WIN32, _WIN64, __CYGWIN__, _MSC_VER 检测)
 * - Linux/Unix (通过 __linux__, __unix__, __APPLE__ 检测)
 * - FreeRTOS (通过 __FREERTOS__, FREERTOS, INC_FREERTOS_H 检测)
 * - RT-Thread (通过 __RTTHREAD__, RT_USING_RTTHREAD, RT_THREAD_H__ 检测)
 * - TencentOS (通过 __TENCENTOS__, TOS_CFG_TASK_PRIO_MAX 检测)
 * 
 * 如需手动指定平台，可在包含本文件前定义 SALOF_OS
 * To manually specify platform, define SALOF_OS before including this file
 */
#ifndef SALOF_OS
    /* Auto-detect Windows platform (自动检测 Windows 平台) */
    #if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(_MSC_VER)
        #define SALOF_OS SALOF_USING_WINDOWS
    /* Auto-detect Linux/Unix platform (自动检测 Linux/Unix 平台) */
    #elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        #define SALOF_OS SALOF_USING_LINUX
    /* Auto-detect FreeRTOS (自动检测 FreeRTOS) */
    #elif defined(__FREERTOS__) || defined(FREERTOS) || defined(INC_FREERTOS_H)
        #define SALOF_OS SALOF_USING_FREERTOS
    /* Auto-detect RT-Thread (自动检测 RT-Thread) */
    #elif defined(__RTTHREAD__) || defined(RT_USING_RTTHREAD) || defined(RT_THREAD_H__)
        #define SALOF_OS SALOF_USING_RTT
    /* Auto-detect TencentOS (自动检测 TencentOS) */
    #elif defined(__TENCENTOS__) || defined(TOS_CFG_TASK_PRIO_MAX)
        #define SALOF_OS SALOF_USING_TENCENTOS
    #endif
#endif

/* Include mqtt_config.h for MQTT-specific settings (can override SALOF_OS if needed) */
#include "mqtt_config.h"

#endif /* _SALOF_CONFIG_H_ */
