#ifndef TUNISMATH_H
#define TUNISMATH_H

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>


namespace tunis
{

namespace detail {
    struct Math
    {
        constexpr static float PI = glm::pi<float>();
        static inline float floor(float v) { return glm::floor(v); }
        static inline float sin(float a) { return glm::sin(a); }
        static inline float cos(float a) { return glm::cos(a); }
        static inline float pow(float x, float y) { return glm::pow(x, y); }
    };
}

constexpr detail::Math Math;



}


#endif // TUNISMATH_H
