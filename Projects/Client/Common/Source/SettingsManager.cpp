#include <SettingsManager.hpp>
#include <QLabel>
#include <QVBoxLayout>
#include <App/Settings.hpp>
#include <QHeaderView>
#include <QMessageBox>
#include <QCoreApplication>
#include <QComboBox>
#include <QPushButton>

namespace RNR
{
    SettingsWidget::SettingsWidget() : QTableWidget()
    {
        Settings* settings = Settings::singleton();
        auto allSettings = settings->getAllSettings();
        verticalHeader()->setVisible(false);
        horizontalHeader()->setVisible(false);
        horizontalHeader()->setStretchLastSection(true);
        setColumnCount(2);
        setRowCount(allSettings.size());
        int setting_count = 0;
        for(auto setting : allSettings)
        {
            pugi::xml_node nodesetting = settings->getSetting((char*)setting.first.c_str());
            QTableWidgetItem* new_property_item = new QTableWidgetItem(tr("%1").arg(setting.first.c_str()));

            Qt::ItemFlags cell_flags = new_property_item->flags();
            cell_flags.setFlag(Qt::ItemFlag::ItemIsEditable, false);
            new_property_item->setFlags(cell_flags);

            QTableWidgetItem* new_property_itemval = 0;
            switch(setting.second.type)
            {
                case PROPERTY_BOOL:
                    {
                        QComboBox* comb = new QComboBox(0);
                        comb->addItem("true");
                        comb->addItem("false");
                        comb->setCurrentIndex(0);
                        comb->setObjectName(setting.first.c_str());
                        connect(comb, &QComboBox::currentTextChanged,
                                this, &SettingsWidget::changedSetting);
                        setCellWidget(setting_count, 1, comb);
                    }
                    break;
                default:
                case PROPERTY_STD_STRING:
                    new_property_itemval = new QTableWidgetItem(tr("%1").arg(nodesetting.text().as_string()));
                    new_property_itemval->setData(Qt::UserRole, QVariant(new_property_item->text()));
                    break;
            }
            new_property_item->setSizeHint(QSize(300,20));
            setItem(setting_count, 0, new_property_item);
            if(new_property_itemval)
                setItem(setting_count, 1, new_property_itemval);
            setting_count++;
        }
        connect(this, &QTableWidget::itemChanged,
                this, &SettingsWidget::changedTSetting);
        resizeRowsToContents();
        resizeColumnsToContents();
    }

    void SettingsWidget::changedSetting(const QString& text)
    {
        QObject* obj = sender();
        QComboBox* box = dynamic_cast<QComboBox*>(obj);
        if(box)
        {
            QString boxname = box->objectName();
            Settings* settings = Settings::singleton();
            pugi::xml_node setting = settings->getSetting(boxname.toLocal8Bit().data());
            setting.set_value(text.toLocal8Bit().data());
            settings->setSetting(setting);
        }
    }

    void SettingsWidget::changedTSetting(QTableWidgetItem* item)
    {
        QString boxname = item->data(Qt::UserRole).toString();
        Settings* settings = Settings::singleton();
        pugi::xml_node setting = settings->getSetting(boxname.toLocal8Bit().data());
        setting.set_value(item->text().toLocal8Bit().data());            
        settings->setSetting(setting);
    }

    ModulesWidget::ModulesWidget() : QWidget()
    {

    }

    SettingsManager::SettingsManager() : QWidget()
    {
        m_tab = new QTabWidget();
        m_tab->setMinimumSize(500,400);
        
        QVBoxLayout* layout = new QVBoxLayout();
        layout->addWidget(m_tab);

        SettingsWidget* settings = new SettingsWidget();
        m_tab->addTab(settings, "Settings");

        ModulesWidget* modules = new ModulesWidget();
        m_tab->addTab(modules, "Modules");

        QPushButton* save = new QPushButton("Save and Close");
        connect(save, &QPushButton::clicked,
                this, &SettingsManager::closeSave);
        layout->addWidget(save, 0, Qt::AlignRight);

        setLayout(layout);
    }

    void SettingsManager::save()
    {
        QMessageBox::warning(this, "RNR", "Please restart RNR for changes to take effect.");

        Settings* settings = Settings::singleton();
        settings->save((QCoreApplication::applicationDirPath() + tr("/settings.xml")).toLocal8Bit().data());
    }

    void SettingsManager::closeSave(bool checked)
    {
        save();
        hide();
    }
}