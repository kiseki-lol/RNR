#include <MainWindow.hpp>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QProgressDialog>
#include <QStatusBar>
#include <App/V8/Tree/ModelInstance.hpp>
#include <App/V8/DataModel/Light.hpp>
#include <Network/NetworkServer.hpp>
#include <Network/NetworkClient.hpp>
#include <App/Script/Script.hpp>
#include <App/Script/ScriptContext.hpp>

MainWindow::MainWindow()
{
    QPixmap pixmap = QPixmap();
    pixmap.loadFromData(icon, icon_size);

    QWidget* content_widget = new QWidget();
    QGridLayout* grid = new QGridLayout();

    ogreRoot = new Ogre::Root();
    Ogre::ConfigDialog* config = OgreBites::getNativeConfigDialog();
    ogreRoot->showConfigDialog(config);

    menubar = new QMenuBar();
    toolbar = new QToolBar();
    createToolbar();

    grid->addWidget(menubar, 0, 0, 1, 3);
    grid->addWidget(toolbar, 1, 0, 1, 3);

    this->ogreWidget = new RNR::OgreWidget(ogreRoot);
    grid->addWidget(this->ogreWidget, 2, 0, 2, 2);
    
    explorer = new QTreeWidget();
    connect(explorer, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(selectInstance(QTreeWidgetItem*, int)));
    grid->addWidget(explorer, 2, 2, 1, 1);

    properties = new PropertyViewer();
    grid->addWidget(properties, 3, 2, 1, 1);

    content_widget->setLayout(grid);

    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);
    setWindowTitle(QString("RNR Studio"));
    setWindowIcon(QIcon(pixmap));
    setCentralWidget(content_widget);

    settings = 0;
}

void MainWindow::widgetItemPrepare(QTreeWidgetItem* item, RNR::Instance* instance)
{
    QString icon_path;
    icon_path = "content/textures/studio/icons/";
    icon_path += instance->getExplorerIcon();
    icon_path += ".png";
    QIcon icon;
    if(QFile::exists(icon_path))
        icon = QIcon(icon_path);
    else
        icon = QIcon("content/textures/studio/icons/Instance.png");
    item->setIcon(0, icon);
}

void MainWindow::selectInstance(QTreeWidgetItem *item, int column)
{
    RNR::Instance* instance = item->data(0, Qt::UserRole).value<RNR::Instance*>();
    ogreWidget->selectedInstance = instance;
    properties->view(instance);
}

void MainWindow::recurseTreeAddInstance(QTreeWidgetItem* parent, RNR::Instance* instance)
{
    for(auto& child : *instance->getChildren())
    {
        QTreeWidgetItem* instance_w = new QTreeWidgetItem();

        widgetItemPrepare(instance_w, child);
        instance_w->setText(0, QString(child->getName().c_str()));
        instance_w->setData(0, Qt::UserRole, QVariant::fromValue(child));

        recurseTreeAddInstance(instance_w, child);
        parent->addChild(instance_w);
    }
}

void MainWindow::updateTree(RNR::Instance* root_instance)
{
    explorer->clear();
    for(auto& child : *root_instance->getChildren())
    {
        QTreeWidgetItem* parent = new QTreeWidgetItem();
        parent->setData(0, Qt::UserRole, QVariant::fromValue(child));
        parent->setText(0, QString(child->getName().c_str()));

        widgetItemPrepare(parent, child);
        recurseTreeAddInstance(parent, child);
        explorer->addTopLevelItem(parent);
    }
}

void MainWindow::updateWorldLoad()
{
    curr_progress->setMaximum(ogreWidget->world->getMaxLoadProgress());
    curr_progress->setValue(ogreWidget->world->getLoadProgress());
    switch(ogreWidget->world->getLoadState())
    {
        case RNR::LOADING_DATAMODEL:
            curr_progress->setLabelText("Loading datamodel");
            break;
        case RNR::LOADING_DATAMODEL_PROPERTIES:
            curr_progress->setLabelText("Loading datamodel properties");
            break;
        case RNR::LOADING_MAKEJOINTS:
            curr_progress->setLabelText("Making joints");
            break;
    }
}

void MainWindow::loadDatamodel()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open RBXL"), tr(""), tr("XML RBXLs (*.rbxl *.rbxlx)"));
    curr_progress = new QProgressDialog("Loading DataModel", "Cancel", 0, 1, this);
    curr_progress->setWindowModality(Qt::WindowModal);
    curr_progress->setAutoClose(false);

    ogreWidget->world->load(filename.toLocal8Bit().data(), this);

    curr_progress->close();
    delete curr_progress;

    printf("MainWindow::loadDataModel: done waiting\n");

    updateTree(ogreWidget->world->getDatamodel());
}

void MainWindow::createToolbar()
{
    QMenu* file_menu = menubar->addMenu("File");
    QAction* load_action = file_menu->addAction("Load", this, SLOT(loadDatamodel()));
    file_menu->addSeparator();
    QAction* settings_action = file_menu->addAction("Settings", this, SLOT(showSettings()));

    QMenu* run_menu = menubar->addMenu("Run");
    QAction* run_script_action = run_menu->addAction("Script", this, SLOT(runScript()));
    QAction* play_solo_action = run_menu->addAction(QIcon("content/textures/studio/icons/play.png"), "Play Solo", this, SLOT(playSolo()));

    QMenu* help_menu = menubar->addMenu("Help");
    help_menu->addAction("About...");

    QAction* run_action = toolbar->addAction(QIcon("content/textures/studio/icons/run.png"), "", this, SLOT(run()));
    QAction* pause_action = toolbar->addAction(QIcon("content/textures/studio/icons/pause.png"), "", this, SLOT(pause()));
    QAction* playSolo_action = toolbar->addAction(QIcon("content/textures/studio/icons/play.png"), "", this, SLOT(playSolo()));

    toolbar->addSeparator();

    QAction* host_action = toolbar->addAction(QIcon("content/textures/studio/icons/host.png"), "Start a server", this, SLOT(startServer()));
    QAction* connect_action = toolbar->addAction(QIcon("content/textures/studio/icons/connect.png"), "Connect to server", this, SLOT(joinClient()));

#ifndef NDEBUG
    toolbar->addSeparator();
    QAction* pointlight = toolbar->addAction(QIcon("content/textures/studio/icons/PointLight.png"), "Debug: Add PointLight to Instance", this, SLOT(dbg_pointlight()));
#endif
}

void MainWindow::showSettings()
{
    if(!settings)
        settings = new RNR::SettingsManager();
    settings->show();
}

void MainWindow::startServer()
{
    RNR::NetworkServer* server = (RNR::NetworkServer*)ogreWidget->world->getDatamodel()->getService("NetworkServer");
    server->start(53640);
    printf("Server started on port 53640\n");
}

void MainWindow::joinClient()
{
    RNR::NetworkClient* client = (RNR::NetworkClient*)ogreWidget->world->getDatamodel()->getService("NetworkClient");
    client->connect("127.0.0.1", 53640);
    //RNR::NetworkReplicator* replicator = dynamic_cast<RNR::NetworkReplicator*>(client->findFirstChild("NetworkReplicator"));
    //if(replicator)
    //    replicator->sendMarker();
}

void MainWindow::run()
{
    this->ogreWidget->world->getRunService()->run();
}

void MainWindow::pause()
{
    this->ogreWidget->world->getRunService()->pause();
}

void MainWindow::playSolo()
{
    this->ogreWidget->world->getRunService()->run();

    RNR::Players* players = (RNR::Players*)this->ogreWidget->world->getDatamodel()->getService("Players");
    RNR::Player* player = players->createLocalPlayer(0);
    if(!player) 
        return;
    player->setName(QInputDialog::getText(this, "Player Name", "Enter your player name").toLocal8Bit().data());
    player->loadCharacter();

    updateTree(ogreWidget->world->getDatamodel());
}

void MainWindow::runScript()
{    
    QString filename = QFileDialog::getOpenFileName(this, tr("Open script"), tr(""), tr("Text files (*.txt *.lua)"));
    QFile file(filename);
    file.open(QFile::ReadOnly);

    RNR::Lua::ScriptContext* context = (RNR::Lua::ScriptContext*)ogreWidget->world->getDatamodel()->getService("ScriptContext");
    RNR::Lua::Script* script = new RNR::Lua::Script(context, file.readAll().toStdString());

    file.close();
    ogreWidget->world->getRunService()->run();
}

void MainWindow::dbg_pointlight()
{
    if(!ogreWidget->selectedInstance)
    {
        QMessageBox::about(this, "selectedInstance = NULL", "Please select an instance in the explorer");
        return;
    }

    printf("MainWindow::dbg_pointlight: inserting Light\n");
    RNR::Light* dbg_light = new RNR::Light();
    dbg_light->setParent(ogreWidget->selectedInstance);

    double r = QInputDialog::getDouble(this, "Red", "Set red component [0.0-1.0]", 1.0, 0.0, 1.0, 2);
    double g = QInputDialog::getDouble(this, "Green", "Set green component [0.0-1.0]", 1.0, 0.0, 1.0, 2);
    double b = QInputDialog::getDouble(this, "Blue", "Set blue component [0.0-1.0]", 1.0, 0.0, 1.0, 2);
    dbg_light->setColor(Ogre::Vector3(r,g,b));

    updateTree(ogreWidget->world->getDatamodel());
}

void MainWindow::closeEvent(QCloseEvent* event)
{

}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QSize new_size = event->size();
    explorer->setMaximumWidth(new_size.width()/4);
    properties->setMaximumWidth(new_size.width()/4);

}