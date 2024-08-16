#pragma once

#ifdef __unix__
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#define sock_errno errno
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#define sock_errno WSAGetLastError()
typedef int socklen_t;

#define MSG_DONTWAIT 0
#define INET_ADDRSTRLEN 16
#endif
