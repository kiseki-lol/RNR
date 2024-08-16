#include <Network/ArkNet/ArkPeer.hpp>
#include <stdexcept>

namespace ArkNet
{
    ArkPeer::ArkPeer(ArkSocket *socket)
    {
        m_socket = socket;
    }

    ArkPeer::ArkPeer(ArkAddress remote, ArkSocket *socket)
    {
        m_remote = remote;
        m_socket = socket;
    }

    ArkPeer::~ArkPeer()
    {
    }

    int ArkPeer::recvFrom(int bytes, char *data, ArkAddress *remote, int flags)
    {
        return m_socket->recvFrom(remote, bytes, data, flags);
    }

    int ArkPeer::sendTo(int bytes, char *data, int flags)
    {
        return m_socket->sendTo(&m_remote, bytes, data, flags);
    }

    void ArkPeer::authorize()
    {
        printf("ArkPeer::authorize: authorized %s\n", m_remote.toString().c_str());
        m_authorized = true;
    }

    ArkPacket *ArkPeer::recvPacket(ArkAddress *remote)
    {
        char packet_buf[65535];
        try
        {
            int sz = recvFrom(65535, packet_buf, remote, 0);
            if (sz == -1)
                return NULL;
            ArkPacket *packet = new ArkPacket();
            packet->data = new char[sz];
            packet->dataSz = sz;
            memcpy(packet->data, packet_buf, packet->dataSz);
            return packet;
        }
        catch (std::runtime_error e)
        {
            printf("ArkPeer::recvPacket: error on receive (what:%s)\n", e.what());
        }
        return NULL;
    }

    void ArkPeer::sendPacket(ArkPacket *packet)
    {
        sendTo(packet->dataSz, packet->data, 0);
    }

    void ArkPeer::clientPump()
    {
        if (m_socket->getSocketStatus() == ARKSOCKET_CONNECTED ||
            m_socket->getSocketStatus() == ARKSOCKET_CONNECTING)
        {
            ArkAddress remote_addr;
            ArkPacket *in_packet = recvPacket(&remote_addr);
            if (in_packet)
            {
                for (auto &listener : m_listeners)
                    listener->onPacketReceiving(this, in_packet);
                delete in_packet;
            }
        }
    }

    void ArkPeer::disconnect(std::string reason, bool silent)
    {
        for(auto& listener : m_listeners)
            listener->onDisconnected(this);
        if(!silent)
            if(m_socket->getSocketType() == ARKSOCKET_CLIENT)
            {
                ArkPacket disconnect_packet(64);
                ArkStream disconnect_stream(&disconnect_packet);
                disconnect_stream.write<char>(0x79);
                disconnect_stream.writeString(reason);
                sendPacket(&disconnect_packet);

                m_socket->markDisconnected();
            }
        else
            if(m_socket->getSocketType() == ARKSOCKET_CLIENT)
                m_socket->markDisconnected();
        if(m_socket->getSocketType() == ARKSOCKET_SERVER)
        {
            ArkPacket disconnect_packet(64);
            ArkStream disconnect_stream(&disconnect_packet);
            disconnect_stream.write<char>(0x79);
            disconnect_stream.writeString(reason);
            sendPacket(&disconnect_packet);
        }
    }
}