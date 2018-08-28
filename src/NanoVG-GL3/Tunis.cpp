#include <Tunis.h>

#define TUNIS_GL_IMPLEMENTATION
#include <../GL/TunisGL.h>

#include <nanovg.h>
#include <nanovg_gl.h>

#include <easy/profiler.h>

NVGcontext* ctx = nullptr;

namespace tunis
{


class Backend
{
public:

    Backend()
    {
        tunisGLInit();
        //ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
        ctx = nvgCreateGL3(0);
    }

    ~Backend()
    {
        nvgDeleteGL3(ctx);
        tunisGLShutdown();
    }

    Color backgroundColor;
};


Context::Context() :
    m_xform(1.0f),
    m_pBackend(new Backend())
{

    setBackgroundColor(color::White);
}

Context::~Context()
{
}

void Context::setBackgroundColor(const Color &color)
{
    if (m_pBackend->backgroundColor != color)
    {
        m_pBackend->backgroundColor = color;
        glClearColor(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
    }
}

void Context::beginFrame(int32_t x, int32_t y, int32_t w, int32_t h)
{
    EASY_FUNCTION(profiler::colors::Teal);

    glViewport(x, y, w, h);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    nvgBeginFrame(ctx, w, h, 1.0f);
}

void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgEndFrame(ctx);
}


void Context::clearRect(float x, float y, float width, float height)
{
    nvgBeginPath(ctx);
    nvgRect(ctx, x, y, width, height);
    nvgFillColor(ctx, nvgRGBA(m_pBackend->backgroundColor.r, m_pBackend->backgroundColor.g, m_pBackend->backgroundColor.b, m_pBackend->backgroundColor.a));
    nvgPathWinding(ctx, NVG_SOLID);
    nvgFill(ctx);
}


void Context::fill(Path2D &path, FillRule fillRule)
{
    EASY_FUNCTION(profiler::colors::Teal);

    nvgBeginPath(ctx);

    size_t pointId = 0;
    for(auto cmd : path.commands())
    {
        switch(cmd)
        {
        case detail::MOVE_TO:
            nvgMoveTo(ctx, path.points()[pointId].x, path.points()[pointId].y);
            ++pointId;
            break;
        case detail::LINE_TO:
            nvgLineTo(ctx, path.points()[pointId].x, path.points()[pointId].y);
            ++pointId;
            break;
        case detail::BEZIER_TO:
            nvgBezierTo(ctx,
                        path.points()[pointId].x,   path.points()[pointId].y,
                        path.points()[pointId+1].x, path.points()[pointId+1].y,
                        path.points()[pointId+2].x, path.points()[pointId+2].y);
            pointId+=3;
            break;
        case detail::CLOSE:
            nvgClosePath(ctx);
            break;
        }
    }

    Color color = fillStyle.getInnerColor();
    nvgFillColor(ctx, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgPathWinding(ctx, fillRule == nonzero ? NVG_SOLID : NVG_HOLE);
    nvgFill(ctx);
}


void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::Teal);

    nvgBeginPath(ctx);

    size_t pointId = 0;
    for(auto cmd : path.commands())
    {
        switch(cmd)
        {
        case detail::MOVE_TO:
            nvgMoveTo(ctx, path.points()[pointId].x, path.points()[pointId].y);
            ++pointId;
            break;
        case detail::LINE_TO:
            nvgLineTo(ctx, path.points()[pointId].x, path.points()[pointId].y);
            ++pointId;
            break;
        case detail::BEZIER_TO:
            nvgBezierTo(ctx,
                        path.points()[pointId].x,   path.points()[pointId].y,
                        path.points()[pointId+1].x, path.points()[pointId+1].y,
                        path.points()[pointId+2].x, path.points()[pointId+2].y);
            pointId+=3;
            break;
        case detail::CLOSE:
            nvgClosePath(ctx);
            break;
        }
    }

    Color color = strokeStyle.getInnerColor();
    nvgStrokeColor(ctx, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgStrokeWidth(ctx, lineWidth);
    nvgStroke(ctx);
}

}
