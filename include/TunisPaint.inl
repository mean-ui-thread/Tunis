#include "TunisPaint.h"

#include <cfloat>

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>

namespace tunis
{

    inline Paint::Paint()
    {
        color() = Black;
        image() = 0;
    }

    inline Paint::Paint(const Color &c)
    {
        color() = c;
        image() = 0;
    }

    inline Paint::Paint(const char* c)
    {
        color() = Color(c);
        image() = 0;
    }

    inline Paint::Paint(const Gradient &gradient)
    {
        // TODO
    }

}
