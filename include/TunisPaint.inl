#include "TunisPaint.h"

#include <cfloat>

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>

namespace tunis
{

    inline Paint::Paint()
    {
        color() = Black;
        image() = detail::blankImage;
    }

    inline Paint::Paint(const Color &c)
    {
        color() = c;
        image() = detail::blankImage;
    }

    inline Paint::Paint(const char* c)
    {
        color() = Color(c);
        image() = detail::blankImage;
    }

    inline Paint::Paint(const Gradient &gradient)
    {
        // TODO
    }

    inline Paint::Paint(const Image &img)
    {
        color() = White;
        image() = img;
    }

}
