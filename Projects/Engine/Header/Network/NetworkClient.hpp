#pragma once
#include <Network/NetworkPeer.hpp>
#include <Network/ArkNet/ArkPeer.hpp>

namespace RNR
{
    class NetworkClient : public NetworkPeer, public ArkNet::IArkPeerListener
    {
    public:
        NetworkClient();

        void connect(char* ip, int port);
        void disconnect();
        void frame();
                
        virtual std::string getClassName() { return "NetworkClient"; }
        virtual void onPacketReceiving(ArkNet::ArkPeer* peer, ArkNet::ArkPacket* packet);        
    };
}