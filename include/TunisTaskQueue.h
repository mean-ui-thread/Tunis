#ifndef TUNISTASKQUEUE_H
#define TUNISTASKQUEUE_H

#include <concurrentqueue.h>
#include <functional>

namespace tunis
{
    namespace detail
    {
        class ContextPriv;
        using Task = std::function<void(ContextPriv*)>;
        extern moodycamel::ConcurrentQueue<Task> taskQueue;

        template<typename T, typename Tp>
        inline void enqueueTask(void(T::*callback)(ContextPriv*), Tp *instance)
        {
            detail::taskQueue.enqueue(std::bind(callback, *instance, std::placeholders::_1));
        }
    }
}

#endif // TUNISTASKQUEUE_H
