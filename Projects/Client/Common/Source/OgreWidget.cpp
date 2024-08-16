#include <OgreWidget.hpp>
#include <QApplication>
#include <QStandardPaths>
#include <filesystem>

#include <OGRE/Bites/OgreSGTechniqueResolverListener.h>
#include <OGRE/OgreDefaultDebugDrawer.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreFontManager.h>
#include <App/V8/DataModel/Lighting.hpp>
#include <App/Settings.hpp>

#ifdef __unix__
#include <qpa/qplatformnativeinterface.h>
#include <X11/Xlib.h>
#endif

// credits to https://github.com/Ekman/Qt-Ogre-Widget
namespace RNR
{
    OgreWidget::OgreWidget(Ogre::Root *root, QWidget *parent) : QWidget(parent)
    {
        this->ogreRoot = root;
        this->setMinimumSize(640, 480);
        this->ogreWindow = NULL;

        this->setAttribute(Qt::WA_OpaquePaintEvent);
        this->setAttribute(Qt::WA_PaintOnScreen);
        this->setMouseTracking(true);
        //this->setCursor(QCursor(Qt::BlankCursor));
        this->setFocusPolicy(Qt::StrongFocus);
        this->selectedInstance = 0;

        inputManager = new QtInputManager(this);
    }

    void OgreWidget::initializeOgre()
    {
        QStringList locations = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
        RNR::Settings* settings = new RNR::Settings((QCoreApplication::applicationDirPath() + QString("/settings.xml")).toLocal8Bit().data());
        Ogre::NameValuePairList options = this->getRenderOptions();

        printf("OgreWidget::initializeOgre: initializing render window\n");
        
        Ogre::RenderSystemList list = ogreRoot->getAvailableRenderers();
        std::string rendersystem_sel = std::string(settings->getSetting("RenderSystem").text().as_string());
        if(rendersystem_sel.size() == 0)
            rendersystem_sel = "OpenGL 3+ Rendering Subsystem";
        for(auto entry : list)
        {
            printf("OgreWidget::initializeOgre: available RenderSystem '%s'\n", entry->getName().c_str());
            if(entry->getName().compare(rendersystem_sel) == 0)
            {
                printf("OgreWidget::initializeOgre: found RenderSystem\n");
                ogreRoot->setRenderSystem(entry);
            }
        }
        ogreRoot->initialise(false);

        Ogre::OverlaySystem* ogreOverlay = new Ogre::OverlaySystem();

        ogreWindow = ogreRoot->createRenderWindow("GLWidget-RenderWindow", width(), height(), false, &options);
        ogreWindow->setActive(true);
        ogreWindow->setVisible(true);
        ogreWindow->setAutoUpdated(true);

        Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("shaders", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        //Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("../Content/OgrePrivate/RTShaderLib/GLSL/", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); // rtshader path
        //Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();

        ogreSceneManager = ogreRoot->createSceneManager();
        ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.2f,0.2f,0.2f));
        ogreSceneManager->addRenderQueueListener(ogreOverlay);

        Ogre::SceneNode* camNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode();
        camNode->setPosition(0, 0, 5);
        camNode->lookAt(Ogre::Vector3(0, sinf(render_time), cosf(render_time)), Ogre::Node::TS_PARENT);

        ogreCamera = ogreSceneManager->createCamera("myCam");
        ogreCamera->setNearClipDistance(0.1); // specific to this sample
        ogreCamera->setFarClipDistance(1000.f);
        ogreCamera->setAutoAspectRatio(true);
        ogreCamera->setFOVy(Ogre::Degree(70.f));
        ogreCamera->setUseRenderingDistance(true);
        camNode->attachObject(ogreCamera);

        ogreViewport = ogreWindow->addViewport(ogreCamera);

        if(Ogre::RTShader::ShaderGenerator::initialize())
        {
            ogreShaderGen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
            if(settings->getSetting("OGREDiskShaderCacheEnable").text().as_bool())
            {
                std::string scPath = (QCoreApplication::applicationDirPath() + tr("/ShaderCache")).toStdString();
                if (!std::filesystem::is_directory(scPath) || !std::filesystem::exists(scPath)) {
                    std::filesystem::create_directory(scPath);
                }

                ogreShaderGen->setShaderCachePath(scPath);
            }
            ogreShaderGen->addSceneManager(ogreSceneManager);

            OgreBites::SGTechniqueResolverListener* technique_resolver = new OgreBites::SGTechniqueResolverListener(ogreShaderGen);
            Ogre::MaterialManager::getSingleton().addListener(technique_resolver);

            printf("OgreWidget::initializeOgre: initialized ShaderGenerator successfully\n");   
        }
        else
            printf("OgreWidget::initializeOgre: unable to initialize ShaderGenerator\n");

        Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation("content", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);        
        Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();

        Ogre::FontPtr pFont = Ogre::FontManager::getSingletonPtr()->create("ComicSans", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        pFont->setType(Ogre::FT_TRUETYPE);
        pFont->setSource("fonts/ComicSans.ttf");
        pFont->setTrueTypeSize(16);
        pFont->load();
        
        ogreSceneManager->setSkyBox(true, "sky/null_plainsky512", 500);

        ogreSceneManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_NONE);
        ogreSceneManager->setShadowFarDistance(500.f);

        ogreSceneLight = ogreSceneManager->createLight("SunLight");
        Ogre::SceneNode* lightNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode();
        lightNode->setPosition(0, 10, 15);
        lightNode->setDirection(-0.25, -0.5, -0.5);
        lightNode->attachObject(ogreSceneLight);

        ogreSceneLight->setCastShadows(false);
        ogreSceneLight->setDiffuseColour(0.9, 0.9, 1.0);
        ogreSceneLight->setSpecularColour(1.0, 1.0, 1.0);
        ogreSceneLight->setType(Ogre::Light::LT_DIRECTIONAL);

        Ogre::MeshManager::getSingletonPtr()->load("meshes/Cube.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Ogre::MeshManager::getSingletonPtr()->load("meshes/Cube_Instanced.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Ogre::MeshManager::getSingletonPtr()->load("meshes/Stud.mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        this->render_time = 0.0;
    }

    void OgreWidget::render()
    {
        if(!world)
            return;

        this->delta = ogreRoot->getTimer()->getMicroseconds() / 1000000.0;
        this->render_time += ogreRoot->getTimer()->getMilliseconds() / 1000.0;
        ogreRoot->getTimer()->reset();

        world->preRender(this->delta);

        if(world->getWorkspace()->getCurrentCamera())
        {
            Camera* cam = world->getWorkspace()->getCurrentCamera();
            ogreCamera->getParentSceneNode()->setPosition(cam->getCFrame().getPosition());
            ogreCamera->getParentSceneNode()->setOrientation(Ogre::Quaternion(cam->getCFrame().getRotation()));
        }

        RNR::Lighting* lighting = (RNR::Lighting*)world->getDatamodel()->getService("Lighting");
        if(lighting)
        {   
            float sunpower = lighting->getSunBrightness();
            Ogre::Vector3 clearColor = lighting->getClearColor();
            ogreViewport->setBackgroundColour(Ogre::ColourValue(clearColor.x, clearColor.y, clearColor.z));
            Ogre::Vector3 shadowColor = lighting->getShadowColor();
            ogreSceneManager->setShadowColour(Ogre::ColourValue(shadowColor.x, shadowColor.y, shadowColor.z));
            Ogre::Vector3 topAmbient = lighting->getTopAmbient();
            ogreSceneLight->setDiffuseColour(Ogre::ColourValue(topAmbient.x, topAmbient.y, topAmbient.z));
            Ogre::Vector3 bottomAmbient = lighting->getBottomAmbient();
            ogreSceneManager->setAmbientLight(Ogre::ColourValue(bottomAmbient.x, bottomAmbient.y, bottomAmbient.z));
            Ogre::Vector3 spotLight = lighting->getSpotLight();
            ogreSceneLight->setSpecularColour(Ogre::ColourValue(spotLight.x * sunpower, spotLight.y * sunpower, spotLight.z * sunpower));
        }
        
        if(isVisible())
            ogreRoot->renderOneFrame(this->delta);
    }

    Ogre::NameValuePairList OgreWidget::getRenderOptions()
    {
        Ogre::NameValuePairList options;

        options["externalWindowHandle"] = getWindowHandle();
        options["FSAA"] = "4";
#ifdef FORCE_ENABLE_VSYNC
        options["vsync"] = "true";
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        options["macAPI"] = "cocoa";
        options["macAPICocoaUseNSView"] = "true";
#endif 

        return options;
    }

    Ogre::String OgreWidget::getWindowHandle()
    {
        Ogre::String windowHandle;
        windowHandle = Ogre::StringConverter::toString((unsigned long)winId());
        printf("OgreWidget::getWindowHandle(): %s\n", windowHandle.c_str());
        return windowHandle;
    }

    void OgreWidget::resizeEvent(QResizeEvent *rEvent)
    {
        QWidget::resizeEvent(rEvent);
        if(ogreWindow)
        {
            QSize size = rEvent->size();
            ogreWindow->resize(size.width(), size.height());
        }
    }

    void OgreWidget::paintEvent(QPaintEvent *pEvent)
    {
        //
    }

    void OgreWidget::keyPressEvent(QKeyEvent *e)
    {
        inputManager->keyEvent(e);
    }

    void OgreWidget::keyReleaseEvent(QKeyEvent *e)
    {
        inputManager->keyEvent(e);
    }

    void OgreWidget::mousePressEvent(QMouseEvent *e)
    {
        inputManager->mouseEvent(e);
    }

    void OgreWidget::mouseReleaseEvent(QMouseEvent *e)
    {
        inputManager->mouseEvent(e);
    }

    void OgreWidget::mouseMoveEvent(QMouseEvent *mEvent)
    {
        inputManager->mouseEvent(mEvent);
    }

    void OgreWidget::closeEvent(QCloseEvent* event)
    {
        delete world;

        ogreWindow->destroy();
        ogreRoot->destroySceneManager(ogreSceneManager);
        ogreRoot->shutdown();
    }

    QPaintEngine* OgreWidget::paintEngine() const
    {
        return 0;
    }
}