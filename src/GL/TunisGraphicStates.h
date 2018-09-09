#ifndef TUNISGRAPHICSTATES_H
#define TUNISGRAPHICSTATES_H

#include <TunisColor.h>
#include <TunisTypes.h>

namespace tunis
{
namespace detail
{
struct GraphicStates
{
    Color backgroundColor = Transparent;
    Viewport viewport = Viewport(0, 0, 100, 100);
    uint32_t textureId = 0;
    uint32_t programId = 0;
    int32_t maxTexSize = 0;
};
extern GraphicStates gfxStates;

}
}

#endif // TUNISGRAPHICSTATES_H
