#include <Network/Marker.hpp>
#include <atomic>

namespace RNR::Network
{
    std::atomic<int> m_markerCount;

    Marker::Marker()
    {
        m_id = m_markerCount++;
    }

    void Marker::fireReturned()
    {
        printf("Marker %i returned\n", m_id);
    }
}