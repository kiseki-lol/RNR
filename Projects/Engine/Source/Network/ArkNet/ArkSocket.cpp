#include <Network/ArkNet/ArkSocket.hpp>
#include <Network/ArkNet/ArkPeer.hpp>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <Network/ArkNet/PlatformNeutralSockets.hpp>

namespace ArkNet
{
#ifdef _WIN32
    static bool winsock_ready;
    static int winsock_count;
#endif

    ArkSocket::ArkSocket()
    {
#ifdef _WIN32
        if (!winsock_ready)
        {
            WSAData wsaData;
            int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (err != 0)
            {
                printf("WSAStartup failed with error: %i\n", err);
            }
            winsock_ready = true;
        }
        winsock_count++;
#endif

        m_socketStatus = ARKSOCKET_DISCONNECTED;
        m_sockFd = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_sockFd < 0)
        {
            printf("ArkSocket::ArkSocket: m_sockFd creation failed\n");
        }

#ifdef _WIN32
        unsigned long non_block_mode = 1;
        ioctlsocket(m_sockFd, FIONBIO, &non_block_mode);
#endif

        memset(&m_clientAddress, 0, sizeof(ArkAddress));
        m_clientAddress.address_size = sizeof(m_clientAddress.address);
        memset(&m_remoteAddress, 0, sizeof(ArkAddress));
        m_remoteAddress.address_size = sizeof(m_clientAddress.address);

        m_localPeer = new ArkPeer(this);
    }

    ArkSocket::~ArkSocket()
    {
        delete m_localPeer;
        closeSocket();
#ifdef _WIN32
        winsock_count--;
        if (winsock_count == 0 && winsock_ready)
        {
            WSACleanup();
            winsock_ready = false;
        }
#endif
    }

    void ArkSocket::bindServer(char *ip, int port)
    {
        m_remoteAddress.address.sin_family = AF_INET;
        m_remoteAddress.address.sin_port = htons(port);
        m_remoteAddress.address.sin_addr.s_addr = inet_addr(ip);

        m_localPeer->setRemote(m_remoteAddress);

        m_socketStatus = ARKSOCKET_CONNECTED;
        m_socketType = ARKSOCKET_SERVER;

        if (bind(m_sockFd, (sockaddr *)&m_remoteAddress.address, sizeof(sockaddr_in)) < 0)
        {
            printf("ArkSocket::bindServer: bind failed\n");
            m_socketStatus = ARKSOCKET_DISCONNECTED;
            return;
        }

        printf("ArkSocket::bindServer: bound on %s:%i\n", ip, port);
    }

    void ArkSocket::tryConnect(char *ip, int port)
    {
        m_clientAddress.address.sin_family = AF_INET;
        m_clientAddress.address.sin_port = htons(port);
        m_clientAddress.address.sin_addr.s_addr = inet_addr(ip);

        m_localPeer->setRemote(m_clientAddress);

        m_socketStatus = ARKSOCKET_CONNECTING;
        m_socketType = ARKSOCKET_CLIENT;

        printf("ArkSocket::tryConnect: connecting to %s:%i\n", ip, port);
    }

    int ArkSocket::recvFrom(ArkAddress *remote, size_t bytes, char *output, int flags)
    {
        char data_buf[65535];
        sockaddr *addr = remote ? (sockaddr *)&(remote->address) : NULL;
        socklen_t *len = remote ? (socklen_t *)&(remote->address_size) : NULL;
        int m = recvfrom(m_sockFd, data_buf, sizeof(data_buf), MSG_DONTWAIT | flags, addr, len);
        if (m == -1 && errno != 11)
            printf("ArkSocket::recvFrom: failed, errno: %i (%s)\n", sock_errno, strerror(sock_errno));
        memcpy(output, data_buf, std::min(sizeof(data_buf), bytes));
        return m;
    }

    int ArkSocket::sendTo(ArkAddress *remote, size_t bytes, char *input, int flags)
    {
        int m = sendto(m_sockFd, input, bytes, MSG_DONTWAIT | flags, (sockaddr *)&remote->address, (socklen_t)sizeof(remote->address));
        if (m == -1)
            printf("ArkSocket::sendTo: failed, errno: %i (%s)\n", sock_errno, strerror(sock_errno));
        return m;
    }

    void ArkSocket::closeSocket()
    {
#ifdef _WIN32
        closesocket(m_sockFd);
#else
        close(m_sockFd);
#endif
    }
}