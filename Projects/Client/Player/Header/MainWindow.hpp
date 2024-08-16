#pragma once

#include <QMainWindow>

#include <OgreWidget.hpp>
#include <OGRE/Ogre.h>

#include "Resource/Player.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow();

        Ogre::Root* ogreRoot;
        RNR::OgreWidget* ogreWidget;
    protected:
        void closeEvent(QCloseEvent* event);
};