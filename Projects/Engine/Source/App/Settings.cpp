#include <App/Settings.hpp>

namespace RNR
{
    static Settings* singletonref = 0;

    Settings::Settings(char* file)
    {
        // init settings
        m_settings["OGREDiskShaderCacheEnable"] = { PROPERTY_BOOL, "Render", "", "true"};
        m_settings["RenderSystem"] = { PROPERTY_STD_STRING, "Render", "This can be either GL, GL3, GLES, DX9, DX11, or Metal", "GL"};

        if(file)
            load(file);
        else
        {
            m_settingsDocument = pugi::xml_document();
            m_settingsNode = m_settingsDocument.append_child("rnr");
            addDefaultSettings();            
        }
        if(!singletonref)
            singletonref = this;
    }

    Settings* Settings::singleton()
    {
        return singletonref;
    }

    Setting Settings::getSettingReflection(char* name)
    {
        auto it = m_settings.find(name);
        if(it == m_settings.end())
        {
            return Setting();
        }
        return it->second;
    }

    pugi::xml_node Settings::getSetting(char* name)
    {
        return m_settingsNode.child(name);
    }
        
    pugi::xml_node Settings::addSetting(char* name)
    {
        return m_settingsNode.append_child(name);
    }

    void Settings::setSetting(pugi::xml_node node)
    {
        pugi::xml_node o = m_settingsNode.child(node.name());
        o.set_value(node.value());
    }

    void Settings::save(char* file)
    {
        printf("Settings::save: saving to %s\n", file);
        m_settingsDocument.save_file(file);
    }

    void Settings::load(char* file)
    {
        printf("Settings::load: loading %s\n", file);
        m_settingsDocument.load_file(file);
        m_settingsNode = m_settingsDocument.child("rnr");
        if(m_settingsNode.empty())
            m_settingsNode = m_settingsDocument.append_child("rnr");
        addDefaultSettings();
    }

    void Settings::addDefaultSettings()
    {
        for(auto& [name, setting] : m_settings)
        {
            pugi::xml_node v = getSetting((char*)name.c_str());
            if(v.empty())
            {
                v = addSetting((char*)name.c_str());                
                printf("Settings::load: adding nonexistant tag %s, setting to %s\n", name.c_str(), setting.def_value);
                v.set_value(setting.def_value);
                setSetting(v);
            }
        }
    }
}