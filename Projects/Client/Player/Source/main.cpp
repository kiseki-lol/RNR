#include <stdio.h>
#include <filesystem>

#include <QApplication>

#include <MainWindow.hpp>

#include <App/V8/DataModel/DataModel.hpp>
#include <Network/NetworkClient.hpp>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    MainWindow window = MainWindow();

    window.show();
    window.ogreWidget->initializeOgre();

    RNR::World* world = new RNR::World(window.ogreWidget->ogreRoot, window.ogreWidget->ogreSceneManager, true);
    window.ogreWidget->setWorld(world);

    RNR::NetworkClient* client = dynamic_cast<RNR::NetworkClient*>(world->getDatamodel()->getService("NetworkClient"));
    client->connect("127.0.0.1",53640);

    while (window.isVisible())
    {
        app.processEvents();
        window.ogreWidget->render();
        world->update();
    }

    app.exit();
}