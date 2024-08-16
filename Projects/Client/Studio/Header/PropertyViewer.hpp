#pragma once
#include <QTableWidget>
#include <QLabel>
#include <App/Script/ReflectionProperty.hpp>
#include <App/V8/Tree/Instance.hpp>

class PropertyViewer : public QWidget 
{
private:
    QTableWidget* prop_table;
    QLabel* prop_label;
public:
    PropertyViewer();
    
    void view(RNR::Instance* instance);
};