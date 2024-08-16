#pragma once
#include <App/V8/Tree/Instance.hpp>
#include <App/V8/DataModel/PartInstance.hpp>
#include <App/V8/World/ComPlicitNgine.hpp>
#include <App/V8/World/JointInstance.hpp>

namespace RNR
{
    class Weld : public JointInstance
    {
        virtual btTypedConstraint* constraint();
    public:
        Weld();
        virtual std::string getClassName() { return "Weld"; }
    };

    class Snap : public Weld
    {
        // they're basically the same
    public:
        Snap();
        virtual std::string getClassName() { return "Snap"; }
        virtual std::string getExplorerIcon() { return "Weld"; }
    };
}