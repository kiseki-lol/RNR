#include <MainWindow.hpp>
#include <OGRE/Bites/OgreBitesConfigDialog.h>

MainWindow::MainWindow()
{
    QPixmap pixmap = QPixmap();
    pixmap.loadFromData(icon, icon_size);

    ogreRoot = new Ogre::Root();
    Ogre::ConfigDialog* config = OgreBites::getNativeConfigDialog();
    ogreRoot->showConfigDialog(config);

    ogreWidget = new RNR::OgreWidget(ogreRoot);

    setCentralWidget(ogreWidget);
    setWindowTitle(QString("RNR"));
    setWindowIcon(QIcon(pixmap));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    //
}