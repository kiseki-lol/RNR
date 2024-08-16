#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>

namespace RNR
{
    class SettingsWidget : public QTableWidget
    {
    public:
        SettingsWidget();
    public slots:
        void changedSetting(const QString& text);
        void changedTSetting(QTableWidgetItem* item);
    };

    class ModulesWidget : public QWidget
    {
    public:
        ModulesWidget();
    };

    class SettingsManager : public QWidget
    {
    public:
        SettingsManager();
    public slots:
        void save();
        void closeSave(bool checked);
    protected:
        QTabWidget* m_tab;
    };
}
