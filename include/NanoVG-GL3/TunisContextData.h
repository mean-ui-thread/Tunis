#ifndef TUNISCONTEXDATA_H
#define TUNISCONTEXDATA_H

#include <TunisGL.h>
#include <nanovg.h>

#include <TunisColor.h>
#include <TunisTypes.h>
#include <TunisPath2D.h>


namespace tunis
{
namespace detail
{
struct GlobalContextData
{
    NVGcontext *ctx = nullptr;
    Color backgroundColor = color::Transparent;
    Viewport viewport = Viewport(0, 0, 100, 100);
};

extern GlobalContextData global;

struct ContextData
{
    NVGcontext *ctx = nullptr;
    Path2D currentPath;

    void pathToNVG(Path2D &path);
};

}
}

#endif // TUNISCONTEXDATA_H
