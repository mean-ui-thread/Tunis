/**
 * MIT License
 *
 * Copyright (c) 2018 Matt Chiasson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/
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
