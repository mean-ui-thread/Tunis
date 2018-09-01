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

    if (!detail::global.ctx)
    {
        tunisGLInit();

        int flags = 0;

//        flags |= NVG_ANTIALIAS;
//        flags |= NVG_STENCIL_STROKES;
#ifndef NDEBUG
        flags |= NVG_DEBUG;
#endif

        detail::global.ctx = nvgCreateGL3(flags);

    }

    data.ctx = detail::global.ctx;
}

Context::~Context()
{
    nvgDeleteGL3(data.ctx);

    data.ctx = nullptr;
    detail::global.ctx = nullptr;

    tunisGLShutdown();
}

}
