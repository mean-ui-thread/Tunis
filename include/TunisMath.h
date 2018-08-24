#ifndef TUNISMATH_H
#define TUNISMATH_H

#include <glm/gtc/constants.hpp>


namespace tunis
{

namespace detail {
    class Math
    {
    public:
       const float PI = glm::pi<float>();
    };
}

const detail::Math Math;



}


#endif // TUNISMATH_H
