#ifndef TUNISVERTEX_H
#define TUNISVERTEX_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace tunis
{

using Position = glm::tvec2<float>;
using TCoord = glm::tvec2<uint16_t>;
using Color = glm::tvec4<uint8_t>;

struct Vertex
{
    Position pos;
    TCoord tcoord;
    Color color;
};

}

#endif // TUNISVERTEX_H
