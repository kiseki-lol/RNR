#include <Network/NetworkReplicator.hpp>
#include <Network/NetworkServer.hpp>
#include <Network/NetworkClient.hpp>
#include <Network/Marker.hpp>
#include <App/V8/Tree/InstanceFactory.hpp>
#include <App/V8/World/World.hpp>
#include <App/V8/DataModel/DataModel.hpp>
#include <Helpers/Strings.hpp>

namespace RNR
{
    NetworkReplicator::NetworkReplicator(ArkNet::ArkPeer *peer, bool serverReplicator) : Instance()
    {
        setName("NetworkReplicator");
        m_peer = peer;
        m_serverReplicator = serverReplicator;
        m_peer->addListener(this);
    }

    void NetworkReplicator::lateInit()
    {
        m_pSentTotal = 0;
        m_pAddedTotal = 0;

        if(m_serverReplicator)
        {
            // client-side, there should be no instances in the tree yet
            // so we will put every old instance in pending
            addInstanceTree(world->getDatamodel());
        }
    }

    void NetworkReplicator::addInstanceTree(Instance* instance)
    {
        if(instance->canReplicate(m_serverReplicator))
            addInstance(instance);
        for(auto& child : *instance->getChildren())
            if(child->canReplicate(m_serverReplicator))
            {
                addInstanceTree(child);
            }
    }

    NetworkReplicator::~NetworkReplicator()
    {
        // TODO: remove listener (should be done asap actually)
    }

    void NetworkReplicator::sendInstance(Instance *instance)
    {
        DataModel *datamodel = world->getDatamodel();
        std::string guid = datamodel->getGuidByInstance(instance);

        ArkNet::ArkPacket packet(64);
        ArkNet::ArkStream dataStream(&packet);

        dataStream.write<int>(1);
        dataStream.writeString(guid);
        dataStream.writeString(instance->getClassName());

        m_peer->sendPacket(&packet);

        sendInstanceProps(instance);
    }

    void NetworkReplicator::sendReflectionProperty(ReflectionProperty prop)
    {
        Instance *instance = (Instance *)prop.object();
        if (instance)
        {
            DataModel *datamodel = world->getDatamodel();
            std::string guid = datamodel->getGuidByInstance(instance);

            ArkNet::ArkPacket packet(64);
            ArkNet::ArkStream dataStream(&packet);

            dataStream.write<int>(3);
            dataStream.writeString(guid);
            dataStream.writeString(prop.name());
            dataStream.write(prop.type());

            switch (prop.type())
            {
            case PROPERTY_STD_STRING:
                dataStream.writeString(*(std::string *)prop.rawGetter());
                break;
            case PROPERTY_BOOL:
            case PROPERTY_BRICKCOLOR:
            case PROPERTY_INTEGER:
                dataStream.write(*(int *)prop.rawGetter());
                break;
            case PROPERTY_VECTOR3:
                dataStream.write(*(Ogre::Vector3 *)prop.rawGetter());
                break;
            case PROPERTY_INSTANCE:
            {
                Instance *i = (Instance *)prop.rawGetter();
                dataStream.writeString(datamodel->getGuidByInstance(i));
            }
            break;
            case PROPERTY_CFRAME:
                dataStream.write(*(CoordinateFrame *)prop.rawGetter());
                break;
            case PROPERTY_FLOAT:
                dataStream.write(*(float *)prop.rawGetter());
                break;
            default:
                break;
            }

            m_peer->sendPacket(&packet);
        }
    }

    void NetworkReplicator::sendInstanceProps(Instance *instance)
    {
        std::vector<ReflectionProperty> props = instance->getProperties();
        for (auto prop : props)
            sendReflectionProperty(prop);
    }

    void NetworkReplicator::readDataPacket(ArkNet::ArkPeer *peer, ArkNet::ArkPacket *packet)
    {
        DataModel *datamodel = world->getDatamodel();
        ArkNet::ArkPacket subPacket = packet->sub(4,packet->dataSz-4);
        ArkNet::ArkStream dataStream(&subPacket);
        bool done = false;
        int p_read = 0;
        do
        {
            int packet_type = dataStream.read<int>();
            switch (packet_type) // TODO: make this completely accurate
            {
            default:
                printf("NetworkReplicator::readDataPacket: read bad dp id %i\n", packet_type);
                break;
            case 0: // stop
                done = true;
                break;
            case 1: // del instance
            {
                Instance *delinst = datamodel->getInstanceByGuid(dataStream.readString());
                if (delinst)
                    delinst->setParent(NULL);
            }
            break;
            case 2: // new instance
            {
                std::string i_guid = dataStream.readString();
                Instance *newinst;
                std::string t_type = dataStream.readString();
                if(t_type == "DataModel")
                {
                    std::string dm_guid = datamodel->getGuidByInstance(datamodel);
                    if(dm_guid != "?")
                        datamodel->removeInstanceByGuid(dm_guid, false);
                    datamodel->registerInstanceByGuid(datamodel, i_guid);
                    printf("NetworkReplicator::readDataPacket: reregistered datamodel as %s\n", i_guid.c_str());
                    break;
                }
                if(datamodel->getInstanceByGuid(i_guid))
                {
                    printf("NetworkReplicator::readDataPacket: attempted to create new instance of pre-existing guid\n");
                    break;
                }                
                Instance *existinst = datamodel->findFirstChildOfType(t_type);
                if (existinst)
                {
                    printf("NetworkReplicator::readDataPacket: reregistering %s guid\n", existinst->getName().c_str());
                    std::string o_guid = datamodel->getGuidByInstance(existinst);
                    if(o_guid != "?")
                        datamodel->removeInstanceByGuid(o_guid, false);
                    newinst = existinst;
                }
                else
                {
                    newinst = InstanceFactory::singleton()->build(t_type);
                    if (!newinst)
                    {
                        printf("NetworkReplicator::readDataPacket: newinst == NULL\n");
                        break;
                    }
                    if(t_type == "Part")
                    {
                        PartInstance* part = (PartInstance*)newinst;
                        part->setAnchored(true);
                    }

                }
                datamodel->registerInstanceByGuid(newinst, i_guid);
            }
            break;
            case 3: // update instance prop
                    // we need to read whatever is in the changepropertyitem even if
                    // the item doesnt map to an actual property, since it'll corrupt
                    // the datastream that reads the packet
            {
                std::string i_guid = dataStream.readString();
                Instance *instance = datamodel->getInstanceByGuid(i_guid);
                if (!instance)
                    printf("NetworkReplicator::readDataPacket: received bad Instance GUID '%s'\n", i_guid.c_str());
                std::string p_name = dataStream.readString();
                ReflectionPropertyType p_type = dataStream.read<ReflectionPropertyType>();
                if(instance)
                {
                    std::vector<ReflectionProperty> props = instance->getProperties();
                    bool foundProp = false;
                    for(int i = 0; i < props.size(); i++)
                    {
                        if(props.at(i).name() == p_name && props.at(i).type() == p_type)
                        {
                            foundProp = true;
                            ReflectionProperty prop = props.at(i);
                            switch (p_type)
                            {
                            case PROPERTY_BOOL:
                            case PROPERTY_BRICKCOLOR:
                            case PROPERTY_INTEGER:
                            {
                                int v = dataStream.read<int>();
                                prop.rawSetter(&v);
                            }
                            break;
                            case PROPERTY_CFRAME:
                            {
                                CoordinateFrame cframe = dataStream.read<CoordinateFrame>();
                                prop.rawSetter(&cframe);
                            }
                            break;
                            case PROPERTY_STD_STRING:
                            {
                                std::string string = dataStream.readString();
                                prop.rawSetter(&string);
                            }
                            break;
                            case PROPERTY_VECTOR3:
                            {
                                Ogre::Vector3 vector = dataStream.read<Ogre::Vector3>();
                                prop.rawSetter(&vector);
                            }
                            break;
                            case PROPERTY_INSTANCE:
                            {
                                std::string guid = dataStream.readString();
                                if(guid == "BadInstanceRef")
                                {
                                    prop.rawSetter(NULL);
                                }
                                else
                                {
                                    Instance *inst = datamodel->getInstanceByGuid(guid);
                                    prop.rawSetter(inst);
                                }
                            }
                            break;
                            case PROPERTY_FLOAT:
                            {
                                float v = dataStream.read<float>();
                                prop.rawSetter(&v);
                            }
                            break;
                            default:
                                printf("NetworkReplicator::readDataPacket: unknown ReflectionPropertyType %i\n", p_type);
                                break;
                            }

                            break;
                        }
                    }

                    if(!foundProp) // we still need to skip whatever data may have been written if the property is not found
                        switch (p_type)
                        {
                        case PROPERTY_BOOL:
                        case PROPERTY_BRICKCOLOR:
                        case PROPERTY_INTEGER:
                        {
                            dataStream.read<int>();
                        }
                        break;
                        case PROPERTY_CFRAME:
                        {
                            dataStream.read<CoordinateFrame>();
                        }
                        break;
                        case PROPERTY_INSTANCE:
                        case PROPERTY_STD_STRING:
                        {
                            dataStream.readString();
                        }
                        break;
                        case PROPERTY_VECTOR3:
                        {
                            dataStream.read<Ogre::Vector3>();
                        }
                        break;
                        case PROPERTY_FLOAT:
                        {
                            dataStream.read<float>();
                        }
                        break;
                        default:
                            printf("NetworkReplicator::readDataPacket: unknown ReflectionPropertyType %i\n", p_type);
                            break;
                        }
                }
            }
            break;
            case 4:
                processMarker(dataStream.read<int>());
                break;
            }
            p_read++;
        } while (!done);
        printf("NetworkReplicator::readDataPacket: read %i entries\n", p_read);
    }

    void NetworkReplicator::onPacketReceiving(ArkNet::ArkPeer *peer, ArkNet::ArkPacket *packet)
    {
        ArkNet::ArkStream packetStream(packet);
        if(packetStream.read<char>() == 0x08)
        {
            ArkNet::ArkPacket dataPacket = packet->sub(1, packet->dataSz-1);
            ArkNet::ArkStream dataStream(&dataPacket); // we need to skip the onPacketReceiving type
            switch (dataStream.read<int>())
            {
            case 0x59: // Data
                readDataPacket(peer, &dataPacket);
                break;
            }
        }
    }

    void NetworkReplicator::processMarker(int id)
    {
        if (m_incomingMarkers.size() > 0)
            if (id == m_incomingMarkers.back()->getId())
            {
                m_incomingMarkers.front()->fireReturned();
                m_incomingMarkers.pop_back();
            }
            else
                printf("NetworkReplicator::processMarker: rejected marker %i, not back incoming marker\n", id);
        else
            printf("NetworkReplicator::processMarker: rejected marker %i, m_incomingMarkers has no markers\n", id);
    }

    void NetworkReplicator::addInstance(Instance* instance)
    {
        DataModel* dm = world->getDatamodel();

        if(dm->getGuidByInstance(instance) == "?")
            dm->registerInstanceByGuid(instance, Strings::random_hex(8), false);

        NewInstanceItem n;
        n.guid = dm->getGuidByInstance(instance);
        n.type = instance->getClassName();
        m_pendingInstances.push_back(n);

        if(n.type != "DataModel")
        {
            std::vector<ReflectionProperty> p = instance->getProperties();
            for(auto& pp : p)
            {
                addProperty(&pp);
            }            
        }
        m_pAddedTotal++;
    }

    void NetworkReplicator::delInstance(Instance* instance)
    {
        DataModel* dm = world->getDatamodel();
        DelInstanceItem n;
        n.guid = dm->getGuidByInstance(instance);
        m_deletingInstances.push_back(n);
        m_pAddedTotal++;
    }

    void NetworkReplicator::addProperty(ReflectionProperty* property)
    {
        DataModel* dm = world->getDatamodel();
        ChangePropertyItem p;
        p.name = property->name();
        p.type = property->type();
        p.guid = dm->getGuidByInstance((Instance*)property->object());
        switch(p.type)
        {
        case PROPERTY_STD_STRING:
            p.value.string = *(std::string*)property->rawGetter();
            break;
        case PROPERTY_VECTOR3:
            p.value.vector3 = *(Ogre::Vector3*)property->rawGetter();
            break;
        case PROPERTY_VECTOR2:
            p.value.vector2 = *(Ogre::Vector2*)property->rawGetter();
            break;
        case PROPERTY_FLOAT:
            p.value.number = *(float*)property->rawGetter();
            break;
        case PROPERTY_BOOL:
        case PROPERTY_BRICKCOLOR:
        case PROPERTY_INTEGER:
            p.value.integer = *(int*)property->rawGetter();
            break;
        case PROPERTY_INSTANCE:
            if(property->rawGetter())
                p.value.string = dm->getGuidByInstance((Instance*)property->rawGetter());
            else
                p.value.string = "BadInstanceRef";
            break;
        case PROPERTY_CFRAME:
            p.value.cframe = *(CoordinateFrame*)property->rawGetter();
            break;
        default:
            printf("NetworkReplicator::addProperty: cannot encode property %s of unknown type %i\n", p.name.c_str(), p.type);
            return;
        }
        m_pendingProperties.push_back(p);
        m_pAddedTotal++;
    }

    void NetworkReplicator::addNewInstancePacket(NewInstanceItem pi, ArkNet::ArkStream* stream)
    {
        printf("NetworkReplicator::sendPending: queuing new %s (%s)\n", pi.guid.c_str(), pi.type.c_str());
        stream->write<int>(2); // new instance
        stream->writeString(pi.guid);
        stream->writeString(pi.type);
    }

    void NetworkReplicator::addPropertyPacket(ChangePropertyItem pc, ArkNet::ArkStream* stream)
    {
        printf("NetworkReplicator::sendPending: queuing change %s.%s\n", pc.guid.c_str(), pc.name.c_str());
        stream->write<int>(3); // update instance prop
        stream->writeString(pc.guid);
        stream->writeString(pc.name);
        stream->write(pc.type);
        switch(pc.type)
        {
        default:
            throw std::runtime_error("bad type");
            break;
        case PROPERTY_BRICKCOLOR:
        case PROPERTY_INTEGER:
        case PROPERTY_BOOL:
            stream->write(pc.value.integer);
            break;
        case PROPERTY_INSTANCE:
        case PROPERTY_STD_STRING:
            stream->writeString(pc.value.string);
            break;
        case PROPERTY_FLOAT:
            stream->write(pc.value.number);
            break;
        case PROPERTY_VECTOR2:
            stream->write(pc.value.vector2);
            break;
        case PROPERTY_VECTOR3:
            stream->write(pc.value.vector3);
            break;
        case PROPERTY_CFRAME:
            stream->write(pc.value.cframe);
            break;
        }
    }

    void NetworkReplicator::addDelInstancePacket(DelInstanceItem pd, ArkNet::ArkStream* stream)
    {
        printf("NetworkReplicator::sendPending: queuing del %s\n", pd.guid.c_str());
        stream->write<int>(1); // delete instance
        stream->writeString(pd.guid);
    }

    void NetworkReplicator::sendPending()
    {
        if(world->getLoadState() != LOADING_FINISHED)
            return;
    
        int p_sent = 0;
        int p_max = 16;
        int p_left = m_deletingInstances.size() + m_pendingInstances.size() + m_pendingProperties.size();
        ArkNet::ArkPacket dataPacket(1024);
        ArkNet::ArkStream dataStream(&dataPacket);

        dataStream.write<char>(0x08);
        dataStream.write<int>(0x59);

        while(p_sent < p_max)
        {
            if(m_deletingInstances.size())
            {
                addDelInstancePacket(m_deletingInstances.front(), &dataStream);
                p_sent++;
                m_deletingInstances.pop_front();
            }
            else
                break;
        }

        while(p_sent < p_max)
        {
            if(m_pendingInstances.size())
            {
                addNewInstancePacket(m_pendingInstances.front(), &dataStream);
                p_sent++;
                m_pendingInstances.pop_front();
            }
            else
                break;
        }

        while(p_sent < p_max)
        {
            if(m_pendingProperties.size())
            {
                addPropertyPacket(m_pendingProperties.front(), &dataStream);
                p_sent++;
                m_pendingProperties.pop_front();
            }
            else
                break;
        }

        dataStream.write<int>(0); // end of datapacket id
        
        if(p_sent != 0)
        {
            m_peer->sendPacket(&dataPacket);
            m_pSentTotal += p_sent;
            printf("NetworkReplicator::sendPending: sent %i entries in datapacket (%0.2f%% done, %i left)\n", p_sent, (m_pSentTotal / m_pAddedTotal) * 100.f, p_left - p_sent);
        }
    }
}