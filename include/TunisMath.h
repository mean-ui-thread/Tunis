#ifndef TUNISMATH_H
#define TUNISMATH_H

#include <glm/gtc/constants.hpp>


namespace tunis
{

namespace detail {
    struct Math
    {
        constexpr static float PI = glm::pi<float>();
    };
}

constexpr detail::Math Math;



}


#endif // TUNISMATH_H
