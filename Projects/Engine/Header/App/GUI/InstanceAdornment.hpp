#pragma once
#include <App/GUI/GuiBase3d.hpp>

namespace RNR
{
    class InstanceAdornment : public GuiBase3d
    {
    private:
        Instance* m_adornee;
    protected:
        virtual void setAdorneeEvent(Instance* adornee) = 0;
    public:
        InstanceAdornment();
        void setAdornee(Instance* adornee) { setAdorneeEvent(adornee); m_adornee = adornee; }
        Instance* getAdornee() { return m_adornee; }
        virtual std::string getClassName() { return "InstanceAdornment"; }
    };
}