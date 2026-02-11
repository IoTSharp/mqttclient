# Windows Platform Support for mqttclient

This directory contains the Windows platform adaptation layer for the mqttclient library.

## Overview

The Windows platform implementation provides the necessary abstractions for:
- Memory allocation (`platform_memory.c/h`)
- Mutex/synchronization (`platform_mutex.c/h`)
- Threading (`platform_thread.c/h`)
- Timers (`platform_timer.c/h`)
- Network sockets (`platform_net_socket.c/h`)

## Requirements

- Windows Vista or later (Windows 7, 8, 10, 11, Server 2008+)
- Visual Studio 2013 or later (recommended: Visual Studio 2019 or 2022)
- CMake 3.10 or later
- Windows SDK (included with Visual Studio)

## Building on Windows

### Using Visual Studio with CMake

1. Open a Visual Studio Command Prompt (Developer Command Prompt)

2. Navigate to the project root directory:
   ```cmd
   cd /d C:\path\to\mqttclient
   ```

3. Create a build directory:
   ```cmd
   mkdir build
   cd build
   ```

4. Generate Visual Studio project files:
   ```cmd
   cmake ..
   ```
   
   Or for a specific Visual Studio version:
   ```cmd
   cmake -G "Visual Studio 16 2019" ..
   ```

5. Build the project:
   ```cmd
   cmake --build . --config Release
   ```
   
   Or open the generated `.sln` file in Visual Studio and build from the IDE.

### Using MinGW

1. Ensure MinGW is installed and in your PATH

2. Create a build directory:
   ```cmd
   mkdir build
   cd build
   ```

3. Generate Makefiles:
   ```cmd
   cmake -G "MinGW Makefiles" ..
   ```

4. Build:
   ```cmd
   mingw32-make
   ```

## Implementation Details

### Mutexes
Windows implementation uses Critical Sections (`CRITICAL_SECTION`) which provide the fastest synchronization for threads within the same process.

### Threads
Windows threads are created using `CreateThread()` API. The implementation includes:
- Thread creation with custom stack size
- Thread synchronization using events
- Proper thread cleanup

### Timers
High-resolution timing is implemented using:
- `QueryPerformanceCounter()` for high-precision timing
- `QueryPerformanceFrequency()` for timer calibration
- Hybrid sleep approach for `platform_timer_usleep()`:
  - Short sleeps (< 1ms): busy-wait for accuracy
  - Longer sleeps: `Sleep()` + busy-wait remainder

### Network Sockets
Windows Sockets (Winsock2) implementation:
- Automatic Winsock initialization (WSAStartup)
- Support for TCP protocol (UDP also available)
- IPv4 and IPv6 support
- Blocking and non-blocking socket modes
- Timeout support for send/receive operations

**Note on TLS/SSL Support:**
- TLS/SSL (mbedtls) is **disabled on Windows** by default to avoid compilation issues
- Windows builds use TCP-only connections (no encryption)
- This means MQTT connections on Windows will be unencrypted
- For production use requiring encryption, consider using a TLS-terminating proxy or VPN
- Linux builds continue to support TLS/SSL via mbedtls

## Linking Requirements

When building applications using this library on Windows, you must link against:
- `ws2_32.lib` - Windows Sockets 2 library

Example compiler command:
```cmd
cl.exe your_app.c mqttclient.lib ws2_32.lib
```

## Notes

1. **Winsock Initialization**: The network socket implementation automatically initializes Winsock when needed. No manual initialization is required by the application.

2. **Winsock Cleanup**: For proper resource cleanup, call `platform_net_socket_cleanup()` at program exit. Example:
   ```c
   #include "platform_net_socket.h"
   
   int main() {
       // Your application code here
       
       // Clean up before exit
       platform_net_socket_cleanup();
       return 0;
   }
   ```
   
3. **Thread Priority**: The Windows implementation currently ignores the `priority` parameter in `platform_thread_init()`. Windows thread priorities can be set separately if needed using `SetThreadPriority()`.

4. **Stack Size**: Thread stack size is respected in the Windows implementation. Default is 0, which uses the system default (typically 1MB).

## Testing

After building, you can run the example programs:

```cmd
cd build\bin\Release
onenet_example.exe
```

Note: Some example programs may have compilation issues unrelated to the Windows platform support.

## Recent Fixes (February 2026)

The following issues have been fixed to support Windows builds:

1. **Compiler Flags**: CMake now uses MSVC-compatible flags (`/W3`, `/O2`) instead of GCC flags on Windows
2. **Logging System**: Added Windows arch implementation for the salof logging system
3. **Thread API**: Implemented Windows-native threading using CRITICAL_SECTION, Windows Semaphore, and CreateThread
4. **Weak Functions**: Fixed `platform_timer_now()` weak function to work with MSVC using `/alternatename` linker directive
5. **Platform Detection**: Automatically detect Windows platform and use SALOF_USING_WINDOWS configuration
6. **Windows SDK Headers**: Fixed header include order to prevent Winsock2 conflicts
   - Added `WIN32_LEAN_AND_MEAN` and `_WINSOCKAPI_` definitions
   - Ensured `winsock2.h` is included before `windows.h`
   - Fixes `IPPROTO_IPV6` undeclared and `ICMP_ERROR_INFO` syntax errors
7. **TLS/SSL Disabled on Windows**: Due to mbedtls compilation issues on Windows
   - TLS support (`nettype_tls.c`) is automatically disabled on Windows
   - Windows builds use TCP-only connections (no encryption)
   - This eliminates numerous mbedtls-related compilation errors
   - Controlled by `MQTT_NETWORK_TYPE_NO_TLS` in `config/mqtt_config.h`
   - Linux builds continue to support TLS via mbedtls
8. **Logging Arch Selection**: Fixed CMakeLists.txt to select correct arch implementation
   - `common/log/CMakeLists.txt` now selects Windows or Linux arch directory based on platform
   - Prevents compilation of Linux pthread code on Windows
   - Eliminates `pthread_t`, `pthread_mutex_t`, `sem_t` undeclared identifier errors
9. **Static Library Linking**: Fixed CMakeLists.txt files to use platform-specific extensions
   - Windows: `.lib` extension for static libraries
   - Linux: `.a` extension for static libraries
   - Eliminates linker errors "cannot open input file mc-log.lib"
   - Applied to all CMakeLists.txt files (common, platform, network, mbedtls, mqttclient)
10. **Signed/Unsigned Warning**: Fixed comparison warning in `common/random.c`
    - Added explicit cast to eliminate signed/unsigned mismatch warning in loop comparison
11. **Example Platform Support**: Fixed examples to work on Windows
    - Added conditional includes for Windows (windows.h) vs Linux (unistd.h, pthread.h)
    - Platform-specific compiler flags in example CMakeLists.txt
    - Windows uses Sleep() and CreateThread(), Linux uses sleep() and pthread
12. **SALOF Platform Constants**: Fixed definition order in `salof_defconfig.h`
    - Platform constants (SALOF_USING_WINDOWS, etc.) now defined before including salof_config.h
    - Ensures mqtt_config.h can use these constants when setting SALOF_OS
    - Added fallback platform detection for when SALOF_OS isn't defined by config
    - Fallback detects Windows via _WIN32, _WIN64, __CYGWIN__, or _MSC_VER macros
    - Fixes examples failing to build due to pthread.h include on Windows
13. **Automatic Platform Detection in common/log**: Enhanced logging system with independent platform detection
    - `salof_config.h` now automatically detects platform at compile time
    - Supports Windows, Linux, macOS, FreeRTOS, RT-Thread, TencentOS
    - No manual SALOF_OS configuration needed - platform is detected automatically
    - Detection uses compiler-defined macros (_WIN32, __linux__, etc.)
    - Works independently of MQTT configuration
    - See `common/log/README.md` for detailed documentation

## Troubleshooting

### Windows SDK Header Errors

If you encounter errors like:
- `error C2065: "IPPROTO_IPV6": 未声明的标识符`
- Syntax errors in `ws2tcpip.h` related to `ICMP_ERROR_INFO`

These are caused by incorrect Windows header include order. The fix ensures:
1. `WIN32_LEAN_AND_MEAN` is defined to minimize Windows header dependencies
2. `_WINSOCKAPI_` prevents old winsock1 from being included
3. `winsock2.h` and `ws2tcpip.h` are included before `windows.h`

### Winsock Redefinition Errors

If you encounter errors like:
- `error C2375: "WSAAsyncGetServByPort": 重定义；不同的链接`
- Similar errors for other WSA functions

These occur when both `winsock.h` and `winsock2.h` are included. The fix:
- Network source files (`nettype_tcp.c`, `network.c`) define `_WINSOCKAPI_` before any includes
- This prevents `windows.h` from automatically including the old `winsock.h`
- Ensures only `winsock2.h` is used throughout the codebase

### Winsock Errors
If you encounter network-related errors, check:
- Windows Firewall settings
- Antivirus software blocking network connections
- Network proxy settings

### Build Errors
- Ensure Windows SDK is properly installed
- Check that `winsock2.h` and `ws2_32.lib` are available
- Verify CMake can find the C compiler

## Compatibility

Tested on:
- Windows 10 (x64)
- Windows 11 (x64)
- Visual Studio 2019
- Visual Studio 2022

## License

This code follows the same Apache License v2.0 as the main mqttclient project.
