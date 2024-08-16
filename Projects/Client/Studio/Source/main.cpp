#include <stdio.h>
#include <filesystem>

#include <QApplication>
#include <QSurfaceFormat>
#include <QStatusBar>
#include <App/V8/World/World.hpp>

#include <MainWindow.hpp>

int main(int argc, char** argv)
{
    QSurfaceFormat format;
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

#ifdef FORCE_ENABLE_VSYNC
    format.setSwapInterval(1);
#else
    format.setSwapInterval(0);
#endif

    QSurfaceFormat::setDefaultFormat(format);
    QApplication app(argc, argv);
    MainWindow window = MainWindow();

    window.show();
    window.ogreWidget->initializeOgre();

    RNR::World* world = new RNR::World(window.ogreWidget->ogreRoot, window.ogreWidget->ogreSceneManager, true);
    window.updateTree(world->getDatamodel());
    window.ogreWidget->setWorld(world);

    RNR::ComPlicitNgine* ngine = world->getComPlicitNgine();

    while (window.isVisible())
    {
        window.statusBar()->showMessage(QString::asprintf("Dt=%f, Rt=%f, Pt=%f, FPS=%f, pFPS=%f", window.ogreWidget->delta, window.ogreWidget->render_time, ngine->getPhysicsTime(), 1 / window.ogreWidget->delta, 1 / ngine->getLastPhysicsDelta()));
        app.processEvents();
        window.ogreWidget->render();
        world->update();
    }

    app.exit();
}