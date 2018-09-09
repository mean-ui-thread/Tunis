#ifndef TUNISVERTEX_H
#define TUNISVERTEX_H

#include <TunisTypes.h>
#include <TunisColor.h>

namespace tunis
{

struct Vertex
{
    Position pos;
    TCoord tcoord;
    Color color;
};

using Index = uint16_t;

}

#endif // TUNISVERTEX_H
