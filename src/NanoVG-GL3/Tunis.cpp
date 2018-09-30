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

    class ContextPriv
    {
    public:
        NVGcontext *nvg = nullptr;
        Path2D currentPath;

        void pathToNVG(Path2D &path)
        {
            nvgBeginPath(nvg);

            for(size_t i = 0; i < path.commands().size(); ++i)
            {
                switch(path.commands().type(i))
                {
                case PathCommandType::close:
                    nvgClosePath(nvg);
                    break;
                case PathCommandType::moveTo:
                    nvgMoveTo(nvg, path.commands().param0(i), path.commands().param1(i));
                    break;
                case PathCommandType::lineTo:
                    nvgLineTo(nvg, path.commands().param0(i), path.commands().param1(i));
                    break;
                case PathCommandType::bezierCurveTo:
                    nvgBezierTo(nvg,
                                path.commands().param0(i), path.commands().param1(i),
                                path.commands().param2(i), path.commands().param3(i),
                                path.commands().param4(i), path.commands().param5(i));
                    break;
                case PathCommandType::quadraticCurveTo:
                    nvgQuadTo(nvg,
                              path.commands().param0(i), path.commands().param1(i),
                              path.commands().param2(i), path.commands().param3(i));
                    break;
                case PathCommandType::arc:
                    nvgArc(nvg,
                           path.commands().param0(i), path.commands().param1(i),
                           path.commands().param2(i), path.commands().param3(i),
                           path.commands().param4(i), path.commands().param5(i) > 0.5f ? NVG_CCW : NVG_CW);
                    break;
                case PathCommandType::arcTo:
                    nvgArcTo(nvg,
                             path.commands().param0(i), path.commands().param1(i),
                             path.commands().param2(i), path.commands().param3(i),
                             path.commands().param4(i));
                    break;
                case PathCommandType::ellipse:
                    // TODO figure out how to use rotation, startAngle, endAngle, anticlockwise here...
                    nvgEllipse(nvg,
                               path.commands().param0(i), path.commands().param1(i),
                               path.commands().param2(i), path.commands().param3(i));
                    break;
                case PathCommandType::rect:
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
    if (!detail::gfxStates.nvg)
    {
        tunisGLInit();

        int flags = 0;

//        flags |= NVG_ANTIALIAS;
//        flags |= NVG_STENCIL_STROKES;
#ifndef NDEBUG
        flags |= NVG_DEBUG;
#endif

        detail::gfxStates.nvg = nvgCreateGL3(flags);

    }

    ctx->nvg = detail::gfxStates.nvg;
}

Context::~Context()
{
    nvgDeleteGL3(ctx->nvg);

    ctx->nvg = nullptr;
    detail::gfxStates.nvg = nullptr;

    tunisGLShutdown();
}

const char * Context::backendName() const
{
    return "NanoVG";
}

void Context::clearFrame(int fbLeft, int fbTop, int fbWidth, int fbHeight, Color backgroundColor)
{
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
    nvgBeginFrame(ctx->nvg,
                  static_cast<float>(winWidth),
                  static_cast<float>(winHeight),
                  devicePixelRatio);
}

void Context::endFrame()
{
    nvgEndFrame(ctx->nvg);
}

void Context::clearRect(float x, float y, float width, float height)
{
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
    ctx->pathToNVG(path);

    Color color = fillStyle.innerColor();
    nvgFillColor(ctx->nvg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgFill(ctx->nvg);
}


void Context::stroke(Path2D &path)
{
    ctx->pathToNVG(path);

    Color color = strokeStyle.innerColor();
    nvgStrokeColor(ctx->nvg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgStrokeWidth(ctx->nvg, lineWidth);
    nvgMiterLimit(ctx->nvg, miterLimit);
    nvgStroke(ctx->nvg);
}

}
