#ifndef TUNISTYPES_H
#define TUNISTYPES_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x3.hpp>

#include <vector>

namespace tunis
{
    using Position = glm::vec2;
    using Point = glm::vec2;
    using TCoord = glm::u16vec2;
    using Color = glm::u8vec4;
    using SVGMatrix = glm::mat2x3;

    struct _do_not_initialize_tag {};

    /*!
     * Non-initialzing allocator. This is useful for std::vector using POD data
     */
    template <typename T> struct NonInitializingAllocator {
        typedef T value_type;
        NonInitializingAllocator() = default;
        template <class U> constexpr NonInitializingAllocator(const NonInitializingAllocator<U>&) noexcept {}
        T* allocate(std::size_t n) {
          if(n > std::size_t(-1) / sizeof(T)) throw std::bad_alloc();
          if(auto p = static_cast<T*>(std::malloc(n*sizeof(T)))) return p;
          throw std::bad_alloc();
        }
        void deallocate(T* p, std::size_t) noexcept { std::free(p); }
        void construct(T, _do_not_initialize_tag) const {}
    };

}

template <class T, class U> bool operator==(const tunis::NonInitializingAllocator<T>&, const tunis::NonInitializingAllocator<U>&) { return true; }
template <class T, class U> bool operator!=(const tunis::NonInitializingAllocator<T>&, const tunis::NonInitializingAllocator<U>&) { return false; }

#endif // TUNISTYPES_H
