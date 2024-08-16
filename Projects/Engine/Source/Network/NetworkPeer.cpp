#include <Network/NetworkPeer.hpp>
#include <Network/NetworkReplicator.hpp>

namespace RNR
{
    NetworkPeer::NetworkPeer()
    {
        m_socket = new ArkNet::ArkSocket();
        m_peer = m_socket->getLocalPeer();
    }

    NetworkPeer::~NetworkPeer()
    {
    }

    void NetworkPeer::addChangedProperty(ReflectionProperty* p)
    {
        for(auto& child : *getChildren())
        {
            NetworkReplicator* replicator = dynamic_cast<NetworkReplicator*>(child);
            if(replicator)
            {
                replicator->addProperty(p);
            }
        }
    }

    void NetworkPeer::addNewInstance(Instance* instance)
    {
        for(auto& child : *getChildren())
        {
            NetworkReplicator* replicator = dynamic_cast<NetworkReplicator*>(child);
            if(replicator)
            {
                replicator->addInstance(instance);
            }
        }
    }

    void NetworkPeer::addDelInstance(Instance* instance)
    {
        for(auto& child : *getChildren())
        {
            NetworkReplicator* replicator = dynamic_cast<NetworkReplicator*>(child);
            if(replicator)
            {
                replicator->delInstance(instance);
            }
        }
    }

    void NetworkPeer::sendPendingReplicates()
    {
        for(auto& child : *getChildren())
        {
            NetworkReplicator* replicator = dynamic_cast<NetworkReplicator*>(child);
            if(replicator)
            {
                replicator->sendPending();
            }
        }
    }
}