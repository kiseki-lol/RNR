#pragma once

#include <boost/shared_ptr.hpp>

#include <App/V8/Tree/Instance.hpp>
#include <Rendering/Adorn.hpp>

namespace RNR
{
    class ForceField
    {
        public:
            ForceField();
            ~ForceField();

            void renderForceField(boost::shared_ptr<RNR::Instance>* descendant, RNR::Adorn* adorn, int cycle);
            virtual std::string getClassName() { return "ForceField"; }
    };
}