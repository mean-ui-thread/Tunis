#include <TunisGradient.h>

#include <glm/vec2.hpp>

namespace tunis
{
    inline Gradient::Gradient(float x0, float y0, float x1, float y1)
    {
        start().x = x0;
        start().y = y0;
        end().x = x1;
        end().y = y1;
        radius() = {};
        colorStops().resize(0);
    }

    inline Gradient::Gradient(float x0, float y0, float r0,
                              float x1, float y1, float r1)
    {
        start().x = x0;
        start().y = y0;
        radius()[0] = r0;
        end().x = x1;
        end().y = y1;
        radius()[1] = r1;
        colorStops().resize(0);
    }

    inline void Gradient::addColorStop(float offset, Color color)
    {
        assert(offset >= 0.0f && offset <= 1.0f);
        colorStops().push(std::move(offset), std::move(color));
    }
}


