#ifndef TUNISCANVASGRADIENT_H
#define TUNISCANVASGRADIENT_H

#include <TunisSOA.h>

#include <glm/vec2.hpp>


namespace tunis
{
    struct CanvasGradient : RefCountedSOA<glm::vec2, glm::vec2>
    {
        glm::vec2 &startPoint() { return get<0>(); }
        glm::vec2 &endPoint() { return get<1>(); }
    };
}

#endif // TUNISCANVASGRADIENT_H
