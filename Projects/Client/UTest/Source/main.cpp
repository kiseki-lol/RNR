#include <stdio.h>
#include <string.h>
#include <filesystem>

#include <App/V8/World/World.hpp>
#include <Network/NetworkClient.hpp>
#include <Network/NetworkServer.hpp>

int main(int argc, char** argv)
{
    Ogre::Root* ogreRoot = new Ogre::Root();
    printf("%i\n",argc);
    if(argc == 1) // UI test
    {

    }
    else if(argc == 2) // CLI test
    {
        Ogre::SceneManager* sceneA = ogreRoot->createSceneManager();
        RNR::World* worldA = new RNR::World(ogreRoot, sceneA);
        Ogre::SceneManager* sceneB = ogreRoot->createSceneManager();
        RNR::World* worldB = new RNR::World(ogreRoot, sceneB);

        if(strcmp(argv[1],"net")==0)
        {
            RNR::PartInstance* testPart = (RNR::PartInstance*)RNR::InstanceFactory::singleton()->build("Part");
            testPart->setParent(worldA->getWorkspace());
            
            RNR::NetworkServer* server = dynamic_cast<RNR::NetworkServer*>(worldA->getDatamodel()->getService("NetworkServer"));
            server->start(53640);
            worldA->getRunService()->run();

            RNR::NetworkClient* client = dynamic_cast<RNR::NetworkClient*>(worldB->getDatamodel()->getService("NetworkClient"));
            client->connect("127.0.0.1",53640);
            worldB->getRunService()->run();

            printf("UTest: simulating 100 ticks\n");
            for(int i = 0; i < 100; i++)
            {
                worldA->update();
                worldB->update();
            }

            while(worldA->getComPlicitNgine()->getPhysicsTicks() < 1000);
            while(worldB->getComPlicitNgine()->getPhysicsTicks() < 1000);

            server->stop();

            delete worldA;
            delete worldB;
        }
    }
}