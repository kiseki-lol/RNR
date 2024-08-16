#include <App/V8/Tree/InstanceFactory.hpp>

namespace RNR
{
    InstanceFactory* InstanceFactory::m_singleton = 0;

    InstanceFactory::InstanceFactory() : m_builders()
    {
        m_singleton = this;
    }

    bool InstanceFactory::registerInstance(std::string key, InstanceFactory::InstanceBuilder builder)
    {
        printf("InstanceFactory::registerInstance: registering %s\n", key.c_str());
        bool ok = m_builders.insert(std::make_pair(key, builder)).second;        
        if(!ok)
            printf("InstanceFactory::registerInstance: could not register\n");
        m_builderNames.push_back(key);
        return ok;
    }

    Instance* InstanceFactory::build(std::string key)
    {
        auto it = m_builders.find(key);
        if(it == m_builders.end()) 
            throw std::runtime_error("Invalid instance " + key);
        return it->second();
    }
}