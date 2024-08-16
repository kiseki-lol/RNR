#pragma once

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QElapsedTimer>
#include <QResizeEvent>
#include <OGRE/Ogre.h>
#include <OGRE/RTShaderSystem/OgreShaderGenerator.h>
#include <App/V8/Tree/Instance.hpp>
#include <App/V8/World/World.hpp>
#include <QtInputManager.hpp>

#include <GL/Adorn.hpp>

namespace RNR
{
    class OgreWidget : public QWidget
    {
        Q_OBJECT

        public:
            OgreWidget(Ogre::Root *root, QWidget* parent = nullptr);

            double delta;
            double render_time;

            Adorn* adorn;

            RNR::World* world;            
            RNR::Instance* selectedInstance;
            Ogre::Root* ogreRoot;
            Ogre::RenderWindow* ogreWindow;
            Ogre::SceneManager* ogreSceneManager;
            Ogre::Camera* ogreCamera;
            Ogre::Viewport* ogreViewport;
            Ogre::Light* ogreSceneLight;
            Ogre::RTShader::ShaderGenerator* ogreShaderGen;

            QCursor cursor;
            
            void render(); 
            void initializeOgre();
            void setWorld(RNR::World* world) { this->world = world; inputManager->setWorld(world); }

            Ogre::NameValuePairList getRenderOptions();
            Ogre::String getWindowHandle();

        protected:
            virtual void paintEvent(QPaintEvent* pEvent);
            virtual void resizeEvent(QResizeEvent* rEvent);
            
            virtual void keyPressEvent(QKeyEvent *event);
            virtual void keyReleaseEvent(QKeyEvent *event);
            virtual void mouseMoveEvent(QMouseEvent *event);
            virtual void mousePressEvent(QMouseEvent *event);
            virtual void mouseReleaseEvent(QMouseEvent *event);

            virtual void closeEvent(QCloseEvent* event);
            virtual QPaintEngine* paintEngine() const;
            
        private:
            QElapsedTimer timer;
            QtInputManager* inputManager;
    };
}
