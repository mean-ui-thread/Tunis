#include "TunisPaint.h"

#include <cfloat>

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>

namespace tunis
{

    inline Paint::Paint()
    {
        type() = detail::PaintType::solid;
        colorStops().resize(1);
        colorStops().color(0) = Black;
        image() = detail::blankImage;
    }

    inline Paint::Paint(const Color &c)
    {
        type() = detail::PaintType::solid;
        colorStops().resize(1);
        colorStops().color(0) = c;
        image() = detail::blankImage;
    }

    inline Paint::Paint(const char* c)
    {
        type() = detail::PaintType::solid;
        colorStops().resize(1);
        colorStops().color(0) = c;
        image() = detail::blankImage;
    }

    inline Paint::Paint(const Gradient &g)
    {
        type() = detail::PaintType::gradient;
        start() = g.start();
        end() = g.end();
        radius() = g.radius();
        colorStops() = g.colorStops();
        image() = detail::blankImage;
    }

    inline Paint::Paint(const Image &img)
    {
        type() = detail::PaintType::image;
        colorStops().resize(1);
        colorStops().color(0) = White;
        image() = img;
    }

}
