#pragma once

#include <App/V8/Tree/Instance.hpp>
#include <App/V8/Tree/InstanceFactory.hpp>
#include <App/V8/DataModel/Workspace.hpp>
#include <App/V8/DataModel/Camera.hpp>
#include <App/V8/DataModel/RunService.hpp>
#include <App/V8/DataModel/DataModel.hpp>
#include <App/V8/World/ComPlicitNgine.hpp>
#include <App/V8/World/JointsService.hpp>
#include <Network/Players.hpp>
#include <App/GUI/TopMenuBar.hpp>
#include <OGRE/Ogre.h>
#include <pugixml.hpp>
#include <stack>
#include "LinearMath/btVector3.h"
#include "btBulletDynamicsCommon.h"

namespace RNR
{
    class IInputManager;
    class PartInstance;

    struct WorldUndeserialized
    {
        Instance* instance;
        Instance* parent;
        pugi::xml_node node;
    };

    enum WorldLoadState
    {
        LOADING_DATAMODEL,
        LOADING_DATAMODEL_PROPERTIES,
        LOADING_MAKEJOINTS,
        LOADING_FINISHED
    };

    class ILoadListener
    {
    public:
        virtual void updateWorldLoad() = 0;
    };

    class World
    {
        private:
            static void globalInit();
            static bool globInit;
            static InstanceFactory* m_instanceFactory;
            bool m_runPhysics;
            bool m_sceneHasRender;
            std::map<std::string, Instance*> m_refs;
            std::stack<WorldUndeserialized> m_undeserialized;
            btDiscreteDynamicsWorld* m_dynamicsWorld;
            DataModel* m_datamodel;
            Workspace* m_workspace;
            RunService* m_runService;
            Players* m_players;
            JointsService* m_joints;
            Ogre::Root* m_ogreRoot;
            Ogre::SceneManager* m_ogreSceneManager;
            TopMenuBar* m_tmb;
            IInputManager* m_inputManager;
            float m_lastDelta;
            ComPlicitNgine* m_ngine;
            enum WorldLoadState m_loadState;
            int m_maxLoadProgress;
            int m_loadProgress;
            ILoadListener* m_loadListener;

            void xmlAddItem(pugi::xml_node node, Instance* parent);
        public:
            Instance* getRef(std::string ref) { return m_refs[ref]; }

            World(Ogre::Root* ogre, Ogre::SceneManager* ogreScene, bool hasRender = false);
            ~World();

            bool getHasRender() { return m_sceneHasRender; }
            void setHasRender(bool doittho) { m_sceneHasRender = doittho; }

            void load(char* path, ILoadListener* loadListener = NULL);
            ILoadListener* getLoadListener() { return m_loadListener; }

            void preRender(float timestep);
            void preStep();
            double step(float timestep);
            void update();

            JointsService* getJointsService() { return m_joints; }
            btDiscreteDynamicsWorld* getDynamicsWorld() { return m_dynamicsWorld; }
            ComPlicitNgine* getComPlicitNgine() { return m_ngine; }
            float getLastDelta() { return m_lastDelta; }
            float getLastPhysicsDelta() { return m_ngine->getLastPhysicsDelta(); }
            DataModel* getDatamodel() { return m_datamodel; }
            void setInputManager(IInputManager* inputManager) { m_inputManager = inputManager; }
            IInputManager* getInputManager() { return m_inputManager; }
            void setDatamodel(DataModel* instance) { m_datamodel = instance; }
            Workspace* getWorkspace() { return m_workspace; }
            void setWorkspace(Workspace* workspace) { m_workspace = workspace; }
            RunService* getRunService() { return m_runService; }
            void setRunService(RunService* runService) { m_runService = runService; }
            Ogre::Root* getOgreRoot() { return m_ogreRoot; }
            Ogre::SceneManager* getOgreSceneManager() { return m_ogreSceneManager; }
            bool getPhysicsShouldBeRunningPleaseStopIfItIsStillRunning() { return m_runPhysics; }
            enum WorldLoadState getLoadState() { return m_loadState; }
            int getMaxLoadProgress() { return m_maxLoadProgress; }
            void setMaxLoadProgress(int p) { m_maxLoadProgress = p; }
            int getLoadProgress() { return m_loadProgress; }
            void setLoadProgress(int p) { m_loadProgress = p; }

            Lock dynamicWorldLock;
    };
}