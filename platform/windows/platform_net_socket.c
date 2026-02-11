/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-01-10 23:45:59
 * @LastEditTime: 2020-06-05 17:13:00
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
#include "platform_net_socket.h"
#include "mqtt_error.h"

// Initialize Winsock - this should be called once at program startup
static volatile LONG winsock_initialized = 0;

static int platform_net_socket_init(void)
{
    // Thread-safe initialization using InterlockedCompareExchange
    if (InterlockedCompareExchange(&winsock_initialized, 1, 0) == 0) {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            InterlockedExchange(&winsock_initialized, 0);
            return -1;
        }
    }
    return 0;
}

int platform_net_socket_connect(const char *host, const char *port, int proto)
{
    SOCKET fd;
    int ret = MQTT_SOCKET_UNKNOWN_HOST_ERROR;
    struct addrinfo hints, *addr_list, *cur;

    // Initialize Winsock
    if (platform_net_socket_init() != 0) {
        return MQTT_SOCKET_FAILED_ERROR;
    }

    /* Do name resolution with both IPv6 and IPv4 */
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = (proto == PLATFORM_NET_PROTO_UDP) ? SOCK_DGRAM : SOCK_STREAM;
    hints.ai_protocol = (proto == PLATFORM_NET_PROTO_UDP) ? IPPROTO_UDP : IPPROTO_TCP;

    if (getaddrinfo(host, port, &hints, &addr_list) != 0) {
        return ret;
    }

    for (cur = addr_list; cur != NULL; cur = cur->ai_next) {
        fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
        if (fd == INVALID_SOCKET) {
            ret = MQTT_SOCKET_FAILED_ERROR;
            continue;
        }

        if (connect(fd, cur->ai_addr, (int)cur->ai_addrlen) == 0) {
            ret = (int)fd;
            break;
        }

        closesocket(fd);
        ret = MQTT_CONNECT_FAILED_ERROR;
    }

    freeaddrinfo(addr_list);
    return ret;
}

int platform_net_socket_recv(int fd, void *buf, size_t len, int flags)
{
    int ret = recv((SOCKET)fd, (char *)buf, (int)len, flags);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK || err == WSAEINPROGRESS) {
            return 0; // Non-blocking socket would block
        }
        return -1;
    }
    return ret;
}

int platform_net_socket_recv_timeout(int fd, unsigned char *buf, int len, int timeout)
{
    int nread;
    int nleft = len;
    unsigned char *ptr;
    ptr = buf;

    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    if (tv.tv_sec < 0 || (tv.tv_sec == 0 && tv.tv_usec <= 0)) {
        tv.tv_sec = 0;
        tv.tv_usec = 100;
    }

    platform_net_socket_setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(struct timeval));

    while (nleft > 0) {
        nread = platform_net_socket_recv(fd, ptr, nleft, 0);
        if (nread < 0) {
            return -1;
        } else if (nread == 0) {
            break;
        }

        nleft -= nread;
        ptr += nread;
    }
    return len - nleft;
}

int platform_net_socket_write(int fd, void *buf, size_t len)
{
    int ret = send((SOCKET)fd, (const char *)buf, (int)len, 0);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK || err == WSAEINPROGRESS) {
            return 0; // Non-blocking socket would block
        }
        return -1;
    }
    return ret;
}

int platform_net_socket_write_timeout(int fd, unsigned char *buf, int len, int timeout)
{
    struct timeval tv;
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    if (tv.tv_sec < 0 || (tv.tv_sec == 0 && tv.tv_usec <= 0)) {
        tv.tv_sec = 0;
        tv.tv_usec = 100;
    }

    setsockopt((SOCKET)fd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&tv, sizeof(struct timeval));
   
    return platform_net_socket_write(fd, buf, len);
}

int platform_net_socket_close(int fd)
{
    return closesocket((SOCKET)fd);
}

int platform_net_socket_set_block(int fd)
{
    u_long mode = 0; // 0 = blocking
    return ioctlsocket((SOCKET)fd, FIONBIO, &mode);
}

int platform_net_socket_set_nonblock(int fd)
{
    u_long mode = 1; // 1 = non-blocking
    return ioctlsocket((SOCKET)fd, FIONBIO, &mode);
}

int platform_net_socket_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    return setsockopt((SOCKET)fd, level, optname, (const char *)optval, optlen);
}

void platform_net_socket_cleanup(void)
{
    if (InterlockedCompareExchange(&winsock_initialized, 0, 1) == 1) {
        WSACleanup();
    }
}
