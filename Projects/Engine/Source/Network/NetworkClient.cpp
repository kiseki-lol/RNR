#include <Network/NetworkClient.hpp>
#include <Network/NetworkReplicator.hpp>

namespace RNR
{
    NetworkClient::NetworkClient()
    {
        setName("NetworkClient");
        m_peer->addListener(this);
    }

    void NetworkClient::connect(char *ip, int port)
    {
        m_socket->tryConnect(ip, port);

        ArkNet::ArkPacket request(64);
        ArkNet::ArkStream stream(&request);
        stream.write<char>(0x0);
        stream.writeString("Player");
        m_peer->sendPacket(&request);

        NetworkReplicator *replicator = new NetworkReplicator(m_peer);
        replicator->setParent(this);
    }

    void NetworkClient::disconnect()
    {
        
    }

    void NetworkClient::frame()
    {
        m_peer->clientPump();
        sendPendingReplicates();
    }

    void NetworkClient::onPacketReceiving(ArkNet::ArkPeer *peer, ArkNet::ArkPacket *packet)
    {
    }
}