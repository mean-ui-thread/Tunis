#include <TunisGradient.h>

#include <glm/vec2.hpp>

namespace tunis
{
    inline Gradient::Gradient(glm::vec2 p0, glm::vec2 p1)
    {
        start() = p0;
        end() = p1;
        radius() = glm::vec2(0);
        colorStops().resize(0);
    }

    inline Gradient::Gradient(glm::vec2 p0, glm::vec2 p1, glm::vec2 r)
    {
        start() = p0;
        end() = p1;
        radius() = r;
        colorStops().resize(0);
    }

    inline void Gradient::addColorStop(float offset, Color color)
    {
        colorStops().push(std::move(offset), std::move(color));
    }
}


