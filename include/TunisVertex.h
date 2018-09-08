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

}

#endif // TUNISVERTEX_H
