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
    GlobalContextData globalContextData;
}

Context::Context()
{

    if (!detail::globalContextData.ctx)
    {
        tunisGLInit();

        int flags = 0;

//        flags |= NVG_ANTIALIAS;
//        flags |= NVG_STENCIL_STROKES;
#ifndef NDEBUG
        flags |= NVG_DEBUG;
#endif

        detail::globalContextData.ctx = nvgCreateGL3(flags);

    }

    data.ctx = detail::globalContextData.ctx;
}

Context::~Context()
{
    nvgDeleteGL3(data.ctx);

    data.ctx = nullptr;
    detail::globalContextData.ctx = nullptr;

    tunisGLShutdown();
}

}
