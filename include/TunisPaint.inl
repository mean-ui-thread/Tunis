#include "TunisPaint.h"

#include <cfloat>

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>

namespace tunis
{

    inline Paint::Paint()
    {
        xform() = SVGMatrix(1.0f);
        extend() = Position(0.0f);
        radius() = 0.0f;
        feather() = 1.0f;
        innerColor() = Black;
        outerColor() = Transparent;
        image() = 0;
    }

    inline Paint::Paint(const Color &color)
    {
        xform() = SVGMatrix(1.0f);
        extend() = Position(0.0f);
        radius() = 0.0f;
        feather() = 1.0f;
        innerColor() = color;
        outerColor() = Transparent;
        image() = 0;
    }

    inline Paint::Paint(const char* colorName)
    {
        xform() = SVGMatrix(1.0f);
        extend() = Position(0.0f);
        radius() = 0.0f;
        feather() = 1.0f;
        innerColor() = Color(colorName);
        outerColor() = Transparent;
        image() = 0;
    }

    inline Paint::Paint(const Gradient &gradient)
    {
        // Calculate transform aligned to the line
        glm::vec2 dir = gradient.end() - gradient.start();
        float length = glm::length(dir);
        if (length > glm::epsilon<float>())
        {
            dir /= length;
        }
        else
        {
            dir = glm::vec2(0, 1);
        }

        Paint paint;
        paint.xform() = {
                dir.y, -dir.x,
                dir.x, dir.y,
                gradient.start().x - dir.x * 1e5,
                gradient.start().y - dir.y * 1e5};
        paint.extend() = {1e5, 1e5 + length * 0.5f};
        paint.radius() = 0.0f;
        paint.feather() = glm::max(1.0f, length);
        paint.innerColor() = Transparent; // TODO color of gradient here
        paint.outerColor() = Transparent;
        paint.image() = 0;
    }

}
