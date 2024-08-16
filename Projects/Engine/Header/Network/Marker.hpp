#pragma once
#include <App/V8/Tree/Instance.hpp>

namespace RNR::Network
{
    class Marker : public Instance
    {
        int m_id;
    public:
        Marker();

        void fireReturned();
        int getId() { return m_id; }
    };
}