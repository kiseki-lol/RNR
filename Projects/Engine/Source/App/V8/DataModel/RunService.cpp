#include <App/V8/DataModel/RunService.hpp>

namespace RNR
{
    RunService::RunService()
    {
        setName("RunService");
        m_time = 0;
        m_running = false;
        m_paused = false;
    }

    void RunService::step(float time)
    {
        if(m_running && !m_paused)
        {
            m_time += time;
        }
    }

    void RunService::run()
    {
        m_running = true;
        m_paused = false;
    }

    void RunService::reset()
    {
        
    }

    void RunService::pause()
    {
        m_paused = true;
    }

    void RunService::unpause()
    {
        m_paused = false;
    }

    void RunService::addProperties(std::vector<ReflectionProperty>& properties)
    {
        ReflectionProperty _properties[]  = {
            { this, std::string("Running"), std::string(""), 
              ACCESS_NONE, OPERATION_READ, PROPERTY_BOOL,         
              REFLECTION_GETTER(RunService* instance = (RunService*)object; return &instance->m_running; ), 
              REFLECTION_SETTER( ) }, // do nothing
            { this, std::string("Paused"), std::string(""), 
              ACCESS_NONE, OPERATION_READ, PROPERTY_BOOL,         
              REFLECTION_GETTER(RunService* instance = (RunService*)object; return &instance->m_paused; ), 
              REFLECTION_SETTER( ) }, // do nothing
        };

        properties.insert(properties.end(), _properties, _properties+(sizeof(_properties)/sizeof(ReflectionProperty)));
    }
}