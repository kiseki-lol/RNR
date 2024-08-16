#pragma once
#include <App/GUI/InstanceAdornment.hpp>

namespace RNR
{
    class SelectionBox : public InstanceAdornment
    {
    protected:
        virtual void setAdorneeEvent(Instance* instance);
    public:
        SelectionBox();
        ~SelectionBox();
        virtual std::string getClassName() { return "SelectionBox"; }
    };
}