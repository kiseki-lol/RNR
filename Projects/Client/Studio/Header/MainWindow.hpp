#pragma once

#include <App/V8/Tree/Instance.hpp>
#include <App/V8/World/World.hpp>

#include <QMainWindow>
#include <QTreeWidget>
#include <QTimer>
#include <QToolBar>
#include <QMenuBar>
#include <QGridLayout>
#include <QTreeView>
#include <QVariant>
#include <QProgressDialog>
#include <PropertyViewer.hpp>
#include <SettingsManager.hpp>

#include <OGRE/Bites/OgreBitesConfigDialog.h>
#include <OgreWidget.hpp>

#include "Resource/Studio.hpp"

class MainWindow : public QMainWindow, public RNR::ILoadListener
{
    Q_OBJECT

    public:
        MainWindow();

        Ogre::Root* ogreRoot;
        RNR::OgreWidget* ogreWidget;
        QTreeWidget* explorer;
        QToolBar* toolbar;
        QMenuBar* menubar;
        PropertyViewer* properties;

        void createToolbar();
        void updateTree(RNR::Instance* root_instance);
        virtual void updateWorldLoad();
    public slots:
        void showSettings();
        void loadDatamodel();
        void selectInstance(QTreeWidgetItem *item, int column);
        void run();
        void pause();
        void playSolo();
        void runScript();

        void startServer();
        void joinClient();

        void dbg_pointlight();
    protected:
        void widgetItemPrepare(QTreeWidgetItem* item, RNR::Instance* instance);
        void recurseTreeAddInstance(QTreeWidgetItem* parent, RNR::Instance* instance);
        void closeEvent(QCloseEvent* event);
        void resizeEvent(QResizeEvent* event);

        QProgressDialog* curr_progress;
        RNR::SettingsManager* settings;
};