#define TUNIS_GL_IMPLEMENTATION
#define NANOVG_GL3_IMPLEMENTATION


#include <TunisGL.h>
#include <nanovg.h>
#include <nanovg_gl.h>

#include <Tunis.h>
#include <TunisGraphicStates.h>

namespace tunis
{
namespace detail
{
    GraphicStates gfxStates;

    struct ContextPriv
    {
        NVGcontext *nvg = nullptr;
        Path2D currentPath;

        void pathToNVG(Path2D &path)
        {
            nvgBeginPath(nvg);

            for(size_t i = 0; i < path.commands().size(); ++i)
            {
                switch(path.commands().type(i))
                {
                case detail::CLOSE:
                    nvgClosePath(nvg);
                    break;
                case detail::MOVE_TO:
                    nvgMoveTo(nvg, path.commands().param0(i), path.commands().param1(i));
                    break;
                case detail::LINE_TO:
                    nvgLineTo(nvg, path.commands().param0(i), path.commands().param1(i));
                    break;
                case detail::BEZIER_TO:
                    nvgBezierTo(nvg,
                                path.commands().param0(i), path.commands().param1(i),
                                path.commands().param2(i), path.commands().param3(i),
                                path.commands().param4(i), path.commands().param5(i));
                    break;
                case detail::QUAD_TO:
                    nvgQuadTo(nvg,
                              path.commands().param0(i), path.commands().param1(i),
                              path.commands().param2(i), path.commands().param3(i));
                    break;
                case detail::ARC:
                    nvgArc(nvg,
                           path.commands().param0(i), path.commands().param1(i),
                           path.commands().param2(i), path.commands().param3(i),
                           path.commands().param4(i), path.commands().param5(i) > 0.5f ? NVG_CCW : NVG_CW);
                    break;
                case detail::ARC_TO:
                    nvgArcTo(nvg,
                             path.commands().param0(i), path.commands().param1(i),
                             path.commands().param2(i), path.commands().param3(i),
                             path.commands().param4(i));
                    break;
                case detail::ELLIPSE:
                    // TODO figure out how to use rotation, startAngle, endAngle, anticlockwise here...
                    nvgEllipse(nvg,
                               path.commands().param0(i), path.commands().param1(i),
                               path.commands().param2(i), path.commands().param3(i));
                    break;
                case detail::RECT:
                    nvgRect(nvg,
                            path.commands().param0(i), path.commands().param1(i),
                            path.commands().param2(i), path.commands().param3(i));
                    break;
                }
            }

            path.reset();
        }
    };

}

Context::Context() :
    ctx(new detail::ContextPriv())
{
    EASY_FUNCTION(profiler::colors::DarkCyan)

    if (!detail::gfxStates.nvg)
    {
        tunisGLInit();

        int flags = 0;

        flags |= NVG_ANTIALIAS;
        flags |= NVG_STENCIL_STROKES;
#ifndef NDEBUG
        flags |= NVG_DEBUG;
#endif

        detail::gfxStates.nvg = nvgCreateGL3(flags);

    }

    ctx->nvg = detail::gfxStates.nvg;
}

Context::~Context()
{
    EASY_FUNCTION(profiler::colors::DarkCyan)

    nvgDeleteGL3(ctx->nvg);

    ctx->nvg = nullptr;
    detail::gfxStates.nvg = nullptr;

    tunisGLShutdown();
}

const char * Context::backendName() const
{
    return "NanoVG (GL3)";
}

void Context::clearFrame(int fbLeft, int fbTop, int fbWidth, int fbHeight, Color backgroundColor)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)

    // update the clear color if necessary
    if (detail::gfxStates.backgroundColor != backgroundColor)
    {
        detail::gfxStates.backgroundColor = backgroundColor;

        glClearColor(backgroundColor.r/255.0f,
                     backgroundColor.g/255.0f,
                     backgroundColor.b/255.0f,
                     backgroundColor.a/255.0f);
    }

    // update the viewport if necessary
    Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
    if (detail::gfxStates.viewport != viewport)
    {
        detail::gfxStates.viewport = viewport;

        glViewport(fbLeft, fbTop, fbWidth, fbHeight);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


void Context::beginFrame(int winWidth, int winHeight, float devicePixelRatio)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)

    nvgBeginFrame(ctx->nvg,
                  static_cast<float>(winWidth),
                  static_cast<float>(winHeight),
                  devicePixelRatio);
}

void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::DarkCyan)

    nvgEndFrame(ctx->nvg);
}

void Context::clearRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)

    nvgBeginPath(ctx->nvg);
    nvgRect(ctx->nvg, x, y, width, height);
    nvgFillColor(ctx->nvg, nvgRGBA(detail::gfxStates.backgroundColor.r,
                                   detail::gfxStates.backgroundColor.g,
                                   detail::gfxStates.backgroundColor.b,
                                   detail::gfxStates.backgroundColor.a));
    nvgPathWinding(ctx->nvg, NVG_SOLID);
    nvgFill(ctx->nvg);
}

void Context::fill(Path2D &path, FillRule)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)

    ctx->pathToNVG(path);

    Color color = fillStyle.innerColor();
    nvgFillColor(ctx->nvg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgFill(ctx->nvg);
}


void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)

    ctx->pathToNVG(path);

    Color color = strokeStyle.innerColor();
    nvgStrokeColor(ctx->nvg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgStrokeWidth(ctx->nvg, lineWidth);
    nvgStroke(ctx->nvg);
}

}
