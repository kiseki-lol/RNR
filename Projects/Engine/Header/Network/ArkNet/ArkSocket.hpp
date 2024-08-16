#pragma once
#include <Network/ArkNet/ArkAddress.hpp>
#include <Network/ArkNet/PlatformNeutralSockets.hpp>

namespace ArkNet
{
    class ArkPeer;

    enum ArkSocketStatus
    {
        ARKSOCKET_DISCONNECTED,
        ARKSOCKET_CONNECTING,
        ARKSOCKET_CONNECTED,
    };
    enum ArkSocketType
    {
        ARKSOCKET_SERVER,
        ARKSOCKET_CLIENT,
    };
    class ArkSocket
    {
    private:
        int m_sockFd;
        ArkSocketStatus m_socketStatus;
        ArkSocketType m_socketType;
        ArkAddress m_clientAddress; // client, server that is being connceted to
        ArkAddress m_remoteAddress; // server, ip that the server is bound to
        ArkPeer* m_localPeer;
    public:
        ArkSocket();
        ~ArkSocket();

        void bindServer(char* ip, int port);
        void tryConnect(char* ip, int port);
        // call on a onConnectionAccepted type event
        void markConnected() { m_socketStatus = ARKSOCKET_CONNECTED; }
        void markDisconnected() { m_socketStatus = ARKSOCKET_DISCONNECTED; }

        int recvFrom(ArkAddress* remote, size_t bytes, char* output, int flags = 0);
        int sendTo(ArkAddress* remote, size_t bytes, char* input, int flags = 0);

        void closeSocket();
        
        ArkPeer* getLocalPeer() { return m_localPeer; };
        ArkAddress getClientAddress() { return m_clientAddress; }
        ArkAddress getRemoteAddress() { return m_remoteAddress; }

        ArkSocketStatus getSocketStatus() { return m_socketStatus; }
        ArkSocketType getSocketType() { return m_socketType; }
    };
}