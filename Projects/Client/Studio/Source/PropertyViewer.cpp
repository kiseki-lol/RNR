#include <PropertyViewer.hpp>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QFile>

#include <App/BrickColor.hpp>

PropertyViewer::PropertyViewer() : QWidget()
{
    prop_table = new QTableWidget();
    prop_table->verticalHeader()->setVisible(false);
    prop_table->horizontalHeader()->setVisible(false);
    prop_table->horizontalHeader()->setStretchLastSection(true);
    prop_table->setColumnCount(2);
    prop_table->setMouseTracking(true);
    prop_table->viewport()->setMouseTracking(true);

    prop_label = new QLabel();
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(prop_label);
    layout->addWidget(prop_table);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    prop_label->setText("Properties");
}

void PropertyViewer::view(RNR::Instance* instance)
{
    prop_label->setText(QString::asprintf("%s Properties (%s)", instance->getName().c_str(), instance->getClassName().c_str()));

    std::vector<RNR::ReflectionProperty> properties = instance->getProperties();

    prop_table->setRowCount(properties.size());

    int property_count = 0;
    for(auto& property : properties)
    {
        QTableWidgetItem* new_property_item = new QTableWidgetItem(tr("%1").arg(property.name().c_str()));

        Qt::ItemFlags cell_flags = new_property_item->flags();
        cell_flags.setFlag(Qt::ItemFlag::ItemIsEditable, false);
        new_property_item->setFlags(cell_flags);
        new_property_item->setToolTip(QString(property.description().c_str()));

        QTableWidgetItem* new_property_itemval = new QTableWidgetItem(tr("%1").arg(property.toString().c_str()));
        new_property_itemval->setToolTip(QString(property.description().c_str()));

        if(property.op() == RNR::OPERATION_READ)
        {
            cell_flags = new_property_itemval->flags();
            cell_flags.setFlag(Qt::ItemFlag::ItemIsEditable, false);
            cell_flags.setFlag(Qt::ItemFlag::ItemIsEnabled, false);
            new_property_itemval->setFlags(cell_flags);
            new_property_item->setFlags(cell_flags);
        }

        switch(property.type())
        {
            case RNR::PROPERTY_BOOL:
                {
                    bool check_state = *(bool*)property.rawGetter();
                    new_property_itemval->setCheckState(check_state ? Qt::Checked : Qt::Unchecked);
                    new_property_itemval->setText("");

                    cell_flags = new_property_itemval->flags();
                    cell_flags.setFlag(Qt::ItemFlag::ItemIsEditable, false);
                    new_property_itemval->setFlags(cell_flags);
                }
                break;
            case RNR::PROPERTY_INSTANCE:
                {
                    cell_flags = new_property_itemval->flags();
                    cell_flags.setFlag(Qt::ItemFlag::ItemIsEditable, false);
                    new_property_itemval->setFlags(cell_flags);

                    RNR::Instance* instance_ref = (RNR::Instance*)property.rawGetter();
                    if(!instance_ref)
                        break;
                    QImage image;
                    QString icon_path;
                    icon_path = "content/textures/studio/icons/";
                    icon_path += instance_ref->getExplorerIcon();
                    icon_path += ".png";
                    if(QFile::exists(icon_path))
                        image = QImage(icon_path);
                    else
                        image = QImage("content/textures/studio/icons/Instance.png");
                    new_property_itemval->setData(Qt::DecorationRole, QPixmap::fromImage(image));
                }
                break;
            case RNR::PROPERTY_BRICKCOLOR:
                {
                    int brickcolor = *(int*)property.rawGetter();
                    Ogre::Vector3 color = RNR::BrickColor::color(brickcolor);
                    new_property_itemval->setBackground(QBrush(QColor(
                        (int)floorf(color.x * 255),
                        (int)floorf(color.y * 255),
                        (int)floorf(color.z * 255)
                    )));
                }
                break;
        }

        prop_table->setItem(property_count, 0, new_property_item);
        prop_table->setItem(property_count, 1, new_property_itemval);
        property_count++;
    }

    prop_table->resizeRowsToContents();
}