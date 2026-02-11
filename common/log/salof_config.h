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


/* Include mqtt_config.h for MQTT-specific settings (can override SALOF_OS if needed) */
#include "mqtt_config.h"

#endif /* _SALOF_CONFIG_H_ */
