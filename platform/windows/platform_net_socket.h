/*
 * @Author: jiejie
 * @Github: https://github.com/IoTSharp
 * @Date: 2026-02-11 00:00:00
 * @LastEditTime: 2026-02-11 03:20:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#ifndef _PLATFORM_NET_SOCKET_H_
#define _PLATFORM_NET_SOCKET_H_

/* 
 * Windows Sockets 2 requires specific header include order and definitions.
 * These must be defined BEFORE including any Windows headers to avoid conflicts.
 */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN  /* Exclude rarely-used services from Windows headers */
#endif

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_  /* Prevent inclusion of winsock.h in windows.h */
#endif

/* 
 * Include Winsock2 headers BEFORE windows.h
 * This is critical for proper compilation with Windows SDK
 */
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PLATFORM_NET_PROTO_TCP  0 /**< The TCP transport protocol */
#define PLATFORM_NET_PROTO_UDP  1 /**< The UDP transport protocol */

int platform_net_socket_connect(const char *host, const char *port, int proto);
int platform_net_socket_recv(int fd, void *buf, size_t len, int flags);
int platform_net_socket_recv_timeout(int fd, unsigned char *buf, int len, int timeout);
int platform_net_socket_write(int fd, void *buf, size_t len);
int platform_net_socket_write_timeout(int fd, unsigned char *buf, int len, int timeout);
int platform_net_socket_close(int fd);
int platform_net_socket_set_block(int fd);
int platform_net_socket_set_nonblock(int fd);
int platform_net_socket_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);
void platform_net_socket_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* _PLATFORM_NET_SOCKET_H_ */
