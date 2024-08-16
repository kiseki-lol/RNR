#pragma once
#include <Network/ArkNet/ArkPeer.hpp>
#include <Network/Player.hpp>
#include <Network/Marker.hpp>
#include <App/V8/Tree/Instance.hpp>
#include <App/CoordinateFrame.hpp>
#include <OGRE/Ogre.h>
#include <list>

namespace RNR
{
    struct MarkerItem;

    struct NewInstanceItem
    {
        std::string guid;
        std::string type;
    };

    struct DelInstanceItem
    {
        std::string guid;
    };

    struct ChangePropertyValue
    {
        std::string string;
        int integer;
        float number;
        Ogre::Vector2 vector2;
        Ogre::Vector3 vector3;
        CoordinateFrame cframe;
    };

    struct ChangePropertyItem
    {
        std::string name;
        std::string guid;
        ReflectionPropertyType type;
        ChangePropertyValue value;
    };

    class NetworkReplicator : public Instance, public ArkNet::IArkPeerListener
    {
        std::list<ChangePropertyItem> m_pendingProperties;
        std::list<NewInstanceItem> m_pendingInstances;
        std::list<DelInstanceItem> m_deletingInstances;
        
        std::vector<Network::Marker*> m_incomingMarkers;
        std::vector<int> m_pendingMarkers;
        ArkNet::ArkPeer* m_peer;
        Player* m_player;

        float m_pSentTotal;
        float m_pAddedTotal;

        bool m_serverReplicator; // if this NetworkReplicator is from a NetworkServer
    public:
        NetworkReplicator(ArkNet::ArkPeer* peer, bool serverReplicator = false);
        ~NetworkReplicator();

        virtual void lateInit();

        void addProperty(ReflectionProperty* property);
        void addInstance(Instance* instance);
        void delInstance(Instance* instance);

        void readDataPacket(ArkNet::ArkPeer* peer, ArkNet::ArkPacket* packet);

        void sendInstance(Instance* instance);
        void sendReflectionProperty(ReflectionProperty prop);
        void sendInstanceProps(Instance* instance);
        void sendDelInstance(Instance* instance);

        void addNewInstancePacket(NewInstanceItem pi, ArkNet::ArkStream* stream);
        void addPropertyPacket(ChangePropertyItem pc, ArkNet::ArkStream* stream);
        void addDelInstancePacket(DelInstanceItem pd, ArkNet::ArkStream* stream);
        void addInstanceTree(Instance* instance);
        void sendPending();

        Network::Marker* sendMarker();
        void processMarker(int id);

        ArkNet::ArkPeer* getPeer() { return m_peer; };
        Player* getPlayer() { return m_player; }
        void setPlayer(Player* player) { m_player = player; }
        virtual void onPacketReceiving(ArkNet::ArkPeer* peer, ArkNet::ArkPacket* packet);        
        virtual std::string getClassName() { return "NetworkReplicator"; }
        virtual bool canReplicate(bool server = false) { return false; };
    };
}