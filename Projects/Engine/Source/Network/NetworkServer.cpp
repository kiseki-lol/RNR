#include <Network/NetworkServer.hpp>
#include <Network/Player.hpp>
#include <App/V8/World/World.hpp>

namespace RNR
{
    NetworkServer::NetworkServer() : NetworkPeer()
    {
        setName("NetworkServer");
        m_server = new ArkNet::ArkServer(m_peer);
        m_server->setListener(this);
        m_peer->addListener(this);
        m_running = false;
    }

    NetworkServer::~NetworkServer()
    {
        stop();
    }

    NetworkReplicator *NetworkServer::getReplicatorForPeer(ArkNet::ArkPeer *peer)
    {
        for (auto &child : *getChildren())
        {
            NetworkReplicator *replicator = dynamic_cast<NetworkReplicator *>(child);
            if (replicator && replicator->getPeer() == peer)
                return replicator;
        }
        return NULL;
    }

    void NetworkServer::onPeerAdding(ArkNet::ArkPeer *addingPeer)
    {
        NetworkReplicator *peer_replicator = new NetworkReplicator(addingPeer, true);
        peer_replicator->setParent(this);
    }

    void NetworkServer::onPeerRemoving(ArkNet::ArkPeer *removingPeer)
    {
        for (auto &child : *getChildren())
        {
            NetworkReplicator *replicator = dynamic_cast<NetworkReplicator *>(child);
            if (replicator)
            {
                replicator->setParent(NULL);
                delete replicator;
            }
        }
    }

    bool NetworkServer::onPeerConnectionRequest(ArkNet::ArkPeer *rqPeer, ArkNet::ArkPacket *rq)
    {
        ArkNet::ArkStream stream(rq);
        std::string playerName = stream.readString();

        if (playerName == "FuzzyPickles")
            return false;

        NetworkReplicator *replicator = getReplicatorForPeer(rqPeer);

        // ok, create player stuff
        Player *player = new Player();
        player->setParent(world->getDatamodel()->getService("Players"));
        player->setName(playerName.c_str());

        replicator->setPlayer(player);

        printf("NetworkServer::onPeerConnectionRequest: new player %s\n", player->getName().c_str());

        return true;
    }

    void NetworkServer::start(int port, int threadTime)
    {
        // TODO: create network thread that times out
        // every frame it should sleep for (threadTime/1000.0)-delta time

        m_socket->bindServer("127.0.0.1", port);
        printf("NetworkServer::start: network server running on port %i\n", port);
        m_running = true;
    }

    void NetworkServer::onPacketReceiving(ArkNet::ArkPeer *peer, ArkNet::ArkPacket *packet)
    {
    }

    void NetworkServer::frame()
    {
        if (m_running)
        {
            sendPendingReplicates();
            m_server->frame();
        }
    }

    void NetworkServer::stop(int timeout)
    {
        m_socket->closeSocket();
        m_running = false;
    }
}