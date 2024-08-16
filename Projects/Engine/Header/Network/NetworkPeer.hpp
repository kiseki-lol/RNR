#pragma once
#include <Network/ArkNet/ArkPeer.hpp>
#include <App/V8/Tree/Instance.hpp>

namespace RNR
{
    class NetworkPeer : public Instance
    {
    protected:
        ArkNet::ArkPeer* m_peer;
        ArkNet::ArkSocket* m_socket;
        bool m_running;
    public:
        NetworkPeer();
        ~NetworkPeer();

        void authorize();
        
        void addChangedProperty(ReflectionProperty* property);
        void addNewInstance(Instance* instance);
        void addDelInstance(Instance* instance);

        void sendPendingReplicates();
        
        virtual std::string getClassName() { return "NetworkPeer"; }
        virtual bool canReplicate(bool server = false) { return false; };
    };
}