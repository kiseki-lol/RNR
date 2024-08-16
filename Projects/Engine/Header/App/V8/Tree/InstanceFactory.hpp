#pragma once
#include <functional>
#include <App/V8/Tree/Instance.hpp>

namespace RNR
{
    class InstanceFactory
    {
    public:
        typedef std::function<Instance*()> InstanceBuilder;
        InstanceFactory();

        bool registerInstance(std::string key, InstanceBuilder builder);
        Instance* build(std::string key);
        std::vector<std::string> getBuilderNames() { return m_builderNames; };

        static InstanceFactory* singleton() { return m_singleton; };

        // default builder
        template <typename Derived>
        static Instance* instanceBuilder() { return new Derived(); }
    private:
        static InstanceFactory* m_singleton;
        std::map<std::string, InstanceBuilder> m_builders;
        std::vector<std::string> m_builderNames;
    };

}
