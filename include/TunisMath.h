#ifndef TUNISMATH_H
#define TUNISMATH_H

#include <glm/gtc/constants.hpp>


namespace tunis
{

namespace detail {
    class Math
    {
    public:
       const double PI = glm::pi<double>();
    };
}

const detail::Math Math;



}


#endif // TUNISMATH_H
