#include <atomic>

namespace RNR
{
    struct Lock {
        std::atomic<bool> lock_ = {false};

        void lock() { while(lock_.exchange(true, std::memory_order_acquire)); }

        void unlock() { lock_.store(false, std::memory_order_release); }
    };
}