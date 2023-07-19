#pragma once

#include <App/V8/Tree/Instance.hpp>
#include <App/V8/Tree/InstanceFactory.hpp>
#include <App/V8/DataModel/Workspace.hpp>
#include <App/V8/DataModel/Camera.hpp>
#include <App/V8/DataModel/RunService.hpp>
#include <App/V8/DataModel/DataModel.hpp>
#include <App/GUI/TopMenuBar.hpp>
#include <OGRE/Ogre.h>
#include <pugixml.hpp>
#include <stack>

namespace RNR
{
    struct WorldUndeserialized
    {
        Instance* instance;
        pugi::xml_node node;
    };

    class World
    {
        private:
            std::map<std::string, Instance*> m_refs;
            std::stack<WorldUndeserialized> m_undeserialized;
            DataModel* m_datamodel;
            Workspace* m_workspace;
            RunService* m_runService;
            Ogre::Root* m_ogreRoot;
            Ogre::SceneManager* m_ogreSceneManager;
            TopMenuBar* m_tmb;
            InstanceFactory* m_instanceFactory;

            void xmlAddItem(pugi::xml_node node, Instance* parent);
        public:
            Instance* getRef(std::string ref) { return m_refs[ref]; }

            World(Ogre::Root* ogre, Ogre::SceneManager* ogreScene);
            ~World();

            void load(char* path);

            void preStep();
            double step(float timestep);
            void update();



            DataModel* getDatamodel() { return m_datamodel; }
            void setDatamodel(DataModel* instance) { m_datamodel = instance; }
            Workspace* getWorkspace() { return m_workspace; }
            void setWorkspace(Workspace* workspace) { m_workspace = workspace; }
            RunService* getRunService() { return m_runService; }
            void setRunService(RunService* runService) { m_runService = runService; }
            Ogre::Root* getOgreRoot() { return m_ogreRoot; }
            Ogre::SceneManager* getOgreSceneManager() { return m_ogreSceneManager; }
    };
}