#ifndef TUNISMATH_H
#define TUNISMATH_H

#include <glm/common.hpp>
#include <glm/gtc/constants.hpp>


namespace tunis
{

namespace detail {
    struct Math
    {
        constexpr static float PI = glm::pi<float>();
        static inline float floor(float v) { return glm::floor(v); }
    };
}

constexpr detail::Math Math;



}


#endif // TUNISMATH_H
