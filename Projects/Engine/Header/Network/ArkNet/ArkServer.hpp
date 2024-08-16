#pragma once
#include <Network/ArkNet/ArkPeer.hpp>
#include <map>

namespace ArkNet
{
    class IArkServerListener
    {
    public:
        virtual void onPeerAdding(ArkPeer* addingPeer) {};
        virtual void onPeerRemoving(ArkPeer* removingPeer) {};
        virtual bool onPeerConnectionRequest(ArkPeer* rqPeer, ArkPacket* rq) { return true; } // returns if connection is accepted;
    };

    class ArkServer : IArkPeerListener
    {
        IArkServerListener* m_serverListener;
        ArkPeer* m_peer;
        std::map<std::string, ArkPeer*> m_peers;
    public:
        ArkServer(ArkPeer* server);
        void setListener(IArkServerListener* listener) { m_serverListener = listener; }
        void onPacketReceiving(ArkPeer* peer, ArkPacket* packet);

        void frame();
    };
}