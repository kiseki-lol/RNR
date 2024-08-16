#pragma once
#include <pugixml.hpp>
#include <App/Script/ReflectionProperty.hpp>
#include <map>
#include <string>

namespace RNR
{
    struct Setting
    {
        bool exists;
        char* group;
        char* desc;
        char* def_value;
        ReflectionPropertyType type;

        Setting() { this->exists = false; }
        Setting(ReflectionPropertyType type, char* group, char* desc, char* def_value)
        {
            this->exists = true;
            this->group = group;
            this->desc = desc;
            this->def_value = def_value;
            this->type = type;
        }
    };

    class Settings
    {
        pugi::xml_document m_settingsDocument;
        pugi::xml_node m_settingsNode;
        std::map<std::string, Setting> m_settings;
    public:
        Settings(char* file = 0);

        std::map<std::string, Setting> getAllSettings() { return m_settings; };
        Setting getSettingReflection(char* name);
        pugi::xml_node getSetting(char* name);
        pugi::xml_node addSetting(char* name);
        void setSetting(pugi::xml_node node);

        void save(char* file);
        void load(char* file);
        void addDefaultSettings();
        
        static Settings* singleton();
    };
}