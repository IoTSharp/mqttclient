# SALOF Logging System

SALOF (Simple Asynchronous Log Output Framework) is a lightweight, platform-independent logging system.

## Features

- **Automatic Platform Detection**: Automatically detects the operating system at compile time
- **Multi-platform Support**: Windows, Linux, macOS, FreeRTOS, RT-Thread, TencentOS
- **Asynchronous Logging**: Non-blocking log output
- **Color Support**: Colored log output for better readability
- **Log Levels**: ERROR, WARNING, INFO, DEBUG
- **Thread-Safe**: Safe for use in multi-threaded environments

## Automatic Platform Detection (自动平台检测)

The logging system automatically detects the platform at compile time without requiring manual configuration.

### Supported Platforms

| Platform | Detection Macros | SALOF_OS Value |
|----------|-----------------|----------------|
| Windows | `_WIN32`, `_WIN64`, `__CYGWIN__`, `_MSC_VER` | `SALOF_USING_WINDOWS` (5) |
| Linux/Unix | `__linux__`, `__unix__`, `__APPLE__` | `SALOF_USING_LINUX` (4) |
| FreeRTOS | `__FREERTOS__`, `FREERTOS`, `INC_FREERTOS_H` | `SALOF_USING_FREERTOS` (2) |
| RT-Thread | `__RTTHREAD__`, `RT_USING_RTTHREAD`, `RT_THREAD_H__` | `SALOF_USING_RTT` (1) |
| TencentOS | `__TENCENTOS__`, `TOS_CFG_TASK_PRIO_MAX` | `SALOF_USING_TENCENTOS` (3) |

### Detection Flow

The platform detection happens in multiple stages for maximum reliability:

1. **Primary Detection** (`salof_config.h`):
   - Checks compiler-defined macros
   - Defines `SALOF_OS` automatically based on detected platform
   
2. **MQTT Integration** (`mqtt_config.h`):
   - Can override platform detection if needed
   - Provides MQTT-specific logging configuration

3. **Fallback Detection** (`salof_defconfig.h`):
   - Final safety check if `SALOF_OS` is still undefined
   - Provides error message if platform cannot be detected

### Manual Override

If you need to manually specify the platform (e.g., cross-compilation), define `SALOF_OS` before including headers:

```c
#define SALOF_OS SALOF_USING_WINDOWS  // Force Windows platform
#include "salof.h"
```

## Usage

### Basic Logging

```c
#include "salof.h"

void example() {
    salof_init();
    
    SALOF_LOG_ERR("Error message");
    SALOF_LOG_WARN("Warning message");
    SALOF_LOG_INFO("Info message");
    SALOF_LOG_DEBUG("Debug message");
}
```

### With MQTT Client

When used with the MQTT client, logging is automatically integrated:

```c
#include "mqttclient.h"

void example() {
    mqtt_log_init();  // Initializes SALOF logging
    
    MQTT_LOG_E("MQTT Error");
    MQTT_LOG_W("MQTT Warning");
    MQTT_LOG_I("MQTT Info");
    MQTT_LOG_D("MQTT Debug");
}
```

## Platform-Specific Implementation

Each platform has its own implementation in the `arch/` directory:

- `arch/windows/` - Windows implementation (CRITICAL_SECTION, Windows threads)
- `arch/linux/` - Linux implementation (pthread, semaphore)

The correct implementation is automatically selected based on the detected platform.

## Configuration

Key configuration options in `salof_defconfig.h`:

- `SALOF_LOG_LEVEL` - Set minimum log level
- `SALOF_LOG_COLOR` - Enable/disable colored output
- `SALOF_LOG_TS` - Enable/disable timestamps
- `SALOF_BUFF_SIZE` - Log buffer size
- `SALOF_FIFO_SIZE` - FIFO queue size

## Building

The logging system builds automatically as part of the mqttclient project:

```bash
mkdir build && cd build
cmake ..
make mc-log
```

On Windows with Visual Studio:
```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Platform Detection Verification

To verify which platform is detected:

```c
#include <stdio.h>
#include "salof_defconfig.h"

void check_platform() {
    #if SALOF_OS == SALOF_USING_WINDOWS
        printf("Platform: Windows\n");
    #elif SALOF_OS == SALOF_USING_LINUX
        printf("Platform: Linux\n");
    #elif SALOF_OS == SALOF_USING_FREERTOS
        printf("Platform: FreeRTOS\n");
    #elif SALOF_OS == SALOF_USING_RTT
        printf("Platform: RT-Thread\n");
    #elif SALOF_OS == SALOF_USING_TENCENTOS
        printf("Platform: TencentOS\n");
    #endif
}
```

## License

See the main project LICENSE file.
