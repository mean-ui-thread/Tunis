#ifndef TUNISPAINT_H
#define TUNISPAINT_H

#include "TunisColor.h"
#include "soa.h"

#include <array>
#include <glm/common.hpp>


namespace tunis
{

class Paint
{
public:

    inline static Paint createLinearGradient(float startX, float startY,
                                             float endX, float endY,
                                             Color innerColor,
                                             Color outerColor)
    {
        {
            float dx, dy, d;
            const float large = 1e5;
            // Calculate transform aligned to the line
            dx = endX - startX;
            dy = endY - startY;
            d = sqrtf(dx*dx + dy*dy);
            if (d > 0.0001f) {
                dx /= d;
                dy /= d;
            } else {
                dx = 0;
                dy = 1;
            }

            Paint p(_soa.size());

            _soa.push_back(
                {dy, -dx, dx, dy, startX - dx*large, startY - dy*large}, // xform
                {large, large + d*0.5f}, // extend
                0.0f,                    // radius
                glm::max(1.0f, d),       // feather
                innerColor,              // innerColor
                outerColor,              // outerColor
                0                        // image
            );

            return p;
        }
    }

    inline Paint(size_t id) : id(id)
    {
    }

    inline float getXform(size_t index) const
    {
        return _soa.get<0>(id)[index];
    }

    inline float getExtend(size_t index) const
    {
        return _soa.get<1>(id)[index];
    }

    inline float getRadius() const
    {
        return _soa.get<2>(id);
    }

    inline float getFeather() const
    {
        return _soa.get<3>(id);
    }

    inline Color getInnerColor() const
    {
        return _soa.get<4>(id);
    }

    inline Color getOuterColor() const
    {
        return _soa.get<5>(id);
    }

    inline size_t getImage() const
    {
        return _soa.get<6>(id);
    }

private:

    static SoA<
        std::array<float, 6>, // xform
        std::array<float, 2>, // extend
        float, // radius
        float, // feather
        Color, // innerColor
        Color, // outerColor
        size_t // image
    > _soa;

    size_t id;
};

}

#endif // TUNISPAINT_H
