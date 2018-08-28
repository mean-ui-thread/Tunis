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

void Context::fillRect(float x, float y, float width, float height)
{
    beginPath();
    rect(x, y, width, height);
    fill();
}


void Context::strokeRect(float x, float y, float width, float height)
{
    beginPath();
    rect(x, y, width, height);
    stroke();
}

void Context::clearRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Teal);

    beginPath();
    rect(x, y, width, height);
    nvgFillColor(ctx, nvgRGBA(m_pBackend->backgroundColor.r, m_pBackend->backgroundColor.g, m_pBackend->backgroundColor.b, m_pBackend->backgroundColor.a));
    nvgPathWinding(ctx, NVG_SOLID);
    nvgFill(ctx);
}


void Context::beginPath()
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgBeginPath(ctx);
}

void Context::closePath()
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgClosePath(ctx);
}

void Context::moveTo(float x, float y)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgMoveTo(ctx, x, y);
}

void Context::lineTo(float x, float y)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgLineTo(ctx, x, y);
}

void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgBezierTo(ctx, cp1x, cp1y, cp2x, cp2y, x, y);
}

void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgQuadTo(ctx, cpx, cpy, x, y);
}

void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgArc(ctx, x, y, radius, startAngle, endAngle, anticlockwise ? NVG_CCW : NVG_CW);

}

void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgArcTo(ctx, x1, y1, x2, y2, radius);
}

void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgEllipse(ctx, x, y, radiusX, radiusY);
}

void Context::rect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgRect(ctx, x, y, width, height);
}

void Context::fill(FillRule fillRule)
{
    EASY_FUNCTION(profiler::colors::Teal);
    Color color = fillStyle.getInnerColor();
    nvgFillColor(ctx, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgPathWinding(ctx, fillRule == nonzero ? NVG_SOLID : NVG_HOLE);
    nvgFill(ctx);
}

void Context::stroke()
{
    EASY_FUNCTION(profiler::colors::Teal);
    Color color = strokeStyle.getInnerColor();
    nvgStrokeColor(ctx, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgStrokeWidth(ctx, lineWidth);
    nvgStroke(ctx);
}

}
