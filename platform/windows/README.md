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
- Support for both TCP and UDP protocols
- IPv4 and IPv6 support
- Blocking and non-blocking socket modes
- Timeout support for send/receive operations

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

## Troubleshooting

### Windows SDK Header Errors

If you encounter errors like:
- `error C2065: "IPPROTO_IPV6": 未声明的标识符`
- Syntax errors in `ws2tcpip.h` related to `ICMP_ERROR_INFO`

These are caused by incorrect Windows header include order. The fix ensures:
1. `WIN32_LEAN_AND_MEAN` is defined to minimize Windows header dependencies
2. `_WINSOCKAPI_` prevents old winsock1 from being included
3. `winsock2.h` and `ws2tcpip.h` are included before `windows.h`

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
