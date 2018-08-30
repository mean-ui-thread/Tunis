#ifndef TUNISPAINT_H
#define TUNISPAINT_H

#include <TunisColor.h>
#include <TunisSOA.h>

#include <algorithm>
#include <array>

#include <glm/common.hpp>

namespace tunis
{

class Paint : public SOA<
        SVGMatrix,
        Position,
        float,
        float,
        Color,
        Color,
        size_t>
{
public:

    inline SVGMatrix &xform() { return get<0>(); }
    inline Position &extend() { return get<1>(); }
    inline float & radius() { return get<2>(); }
    inline float & feather() { return get<3>(); }
    inline Color & innerColor() { return get<4>(); }
    inline Color & outerColor() { return get<5>(); }
    inline size_t & image() { return get<6>(); }

    inline static Paint createLinearGradient(float startX, float startY,
                                             float endX, float endY,
                                             Color innerColor,
                                             Color outerColor)
    {
        float dx, dy, d;
        const float large = 1e5;
        // Calculate transform aligned to the line
        dx = endX - startX;
        dy = endY - startY;
        d = sqrtf(dx*dx + dy*dy);
        if (d > 0.0001f)
        {
            dx /= d;
            dy /= d;
        }
        else
        {
            dx = 0;
            dy = 1;
        }

        Paint paint;
        paint.xform() = {dy, -dx, dx, dy, startX - dx*large, startY - dy*large};
        paint.extend() = {large, large + d*0.5f};
        paint.radius() = 0.0f;
        paint.feather() = glm::max(1.0f, d);
        paint.innerColor() = innerColor;
        paint.outerColor() = outerColor;
        paint.image() = 0;

        return paint;
    }

    inline Paint()
    {
        reset();
    }

    inline Paint(const Paint &other) : SOA(other)
    {
    }

    inline Paint(const Color &color) : SOA()
    {
        xform() = SVGMatrix(1.0f);
        extend() = Position(0.0f);
        radius() = 0.0f;
        feather() = 1.0f;
        innerColor() = color;
        outerColor() = color::Transparent;
        image() = 0;
    }

    inline Paint &operator=(const Color &color)
    {
        return operator=(Paint(color));
    }

    inline void reset()
    {
        xform() = SVGMatrix(1.0f);
        extend() = Position(0.0f);
        radius() = 0.0f;
        feather() = 1.0f;
        innerColor() = color::Black;
        outerColor() = color::Transparent;
        image() = 0;
    }

};

}

#endif // TUNISPAINT_H
