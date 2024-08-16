#pragma once

#include <Helpers/NormalId.hpp>
#include <App/V8/Tree/Instance.hpp>

namespace RNR
{
    class FaceInstance : public Instance
    {
    private:
        NormalId m_face;

    public:
        FaceInstance();

        virtual void build() = 0;
    };
}