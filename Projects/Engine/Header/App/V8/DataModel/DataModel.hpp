#pragma once
#include <App/V8/Tree/Instance.hpp>

namespace RNR
{
    class DataModel : public Instance
    {
        std::map<std::string, Instance*> m_guids;
    public:
        DataModel();            
        
        Instance* getService(std::string service_name);
        virtual std::string getClassName() { return "DataModel"; }
        virtual void onDescendantAdded(RNR::Instance* childAdded);
        virtual void onDescendantRemoved(RNR::Instance* childRemoved);

        Instance* getInstanceByGuid(std::string guid);
        void removeInstanceByGuid(std::string guid, bool replicate = false);
        void registerInstanceByGuid(Instance* instance, std::string guid, bool replicate = false);
        std::string getGuidByInstance(Instance* instance);
    private:
        virtual void addFunctions(std::vector<ReflectionFunction>& functions);
    };
}