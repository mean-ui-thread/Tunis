#ifndef TUNISTYPES_H
#define TUNISTYPES_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x3.hpp>

#include <vector>

namespace tunis
{
    using Color = glm::u8vec4;
    using Point = glm::dvec3;
    using Position = glm::vec2;
    using SVGMatrix = glm::mat2x3;
    using TCoord = glm::u16vec2;
    using Viewport = glm::ivec4;
}

#endif // TUNISTYPES_H
