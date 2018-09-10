#ifndef TUNISSPINLOCK_H
#define TUNISSPINLOCK_H

#include <atomic>
#include <thread>

namespace tunis
{

class SpinLock
{
public:

    SpinLock() {}
    SpinLock(const SpinLock &) {}
    ~SpinLock() {}
    SpinLock &operator=(const SpinLock &) {return *this;}

    void lock()
    {
        while (true)
        {
            for (int i = 0; i < 10000; ++i)
            {
                if (!m_lock.test_and_set(std::memory_order_acquire))
                {
                    return;
                }
            }
            std::this_thread::yield();
        }
    }

    bool try_lock()
    {
        return !m_lock.test_and_set(std::memory_order_acquire);
    }

    void unlock()
    {
        m_lock.clear(std::memory_order_release);
    }

private:

    std::atomic_flag m_lock;
};

}

#endif // TUNISSPINLOCK_H
