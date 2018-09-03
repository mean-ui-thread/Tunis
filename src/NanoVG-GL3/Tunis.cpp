#define TUNIS_GL_IMPLEMENTATION
#include <TunisGL.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg.h>
#include <nanovg_gl.h>

#include <Tunis.h>

namespace tunis
{
namespace detail
{
    GlobalContextData global;
}

Context::Context()
{

    if (!detail::global.nvg)
    {
        tunisGLInit();

        int flags = 0;

//        flags |= NVG_ANTIALIAS;
//        flags |= NVG_STENCIL_STROKES;
#ifndef NDEBUG
        flags |= NVG_DEBUG;
#endif

        detail::global.nvg = nvgCreateGL3(flags);

    }

    ctx.nvg = detail::global.nvg;
}

Context::~Context()
{
    nvgDeleteGL3(ctx.nvg);

    ctx.nvg = nullptr;
    detail::global.nvg = nullptr;

    tunisGLShutdown();
}

}
