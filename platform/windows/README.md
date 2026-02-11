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
- CMake 2.8 or later
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

2. **Thread Priority**: The Windows implementation currently ignores the `priority` parameter in `platform_thread_init()`. Windows thread priorities can be set separately if needed using `SetThreadPriority()`.

3. **Stack Size**: Thread stack size is respected in the Windows implementation. Default is 0, which uses the system default (typically 1MB).

4. **Cleanup**: Winsock cleanup (`WSACleanup()`) is not automatically called. For proper cleanup in production applications, consider adding cleanup code at program exit.

## Testing

After building, you can run the example programs:

```cmd
cd build\bin
emqx.exe
```

## Troubleshooting

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
