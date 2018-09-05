#include <Tunis.h>

namespace tunis
{



inline void Context::clearFrame(int32_t fbLeft, int32_t fbTop, int32_t fbWidth, int32_t fbHeight, Color backgroundColor)
{
    EASY_FUNCTION(profiler::colors::Coral);

    // update the clear color if necessary
    if (detail::gfxStates.backgroundColor != backgroundColor)
    {
        glClearColor(backgroundColor.r/255.0f,
                     backgroundColor.g/255.0f,
                     backgroundColor.b/255.0f,
                     backgroundColor.a/255.0f);

        detail::gfxStates.backgroundColor = backgroundColor;
    }

    // update the viewport if necessary
    Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
    if (detail::gfxStates.viewport != viewport)
    {
        glViewport(fbLeft, fbTop, fbWidth, fbHeight);
        detail::gfxStates.viewport = viewport;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

inline void Context::beginFrame(int32_t winWidth, int32_t winHeight, float devicePixelRatio)
{
    EASY_FUNCTION(profiler::colors::Coral);

    // reset the states.
    ctx.states.resize(0);
    *static_cast<ContextState*>(this) = ContextState();

    ctx.renderViewport(std::move(winWidth), std::move(winHeight), std::move(devicePixelRatio));
}

inline void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.renderFlush();
}

inline void Context::save()
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.states.push_back(*this);
}

inline void Context::restore()
{
    EASY_FUNCTION(profiler::colors::Coral);
    if (ctx.states.size() > 0)
    {
        *static_cast<ContextState*>(this) = ctx.states.back();
        ctx.states.pop_back();
    }
}

inline void Context::fillRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.pushColorRect(std::move(x), std::move(y), std::move(width), std::move(height), fillStyle.innerColor());
}

inline void Context::strokeRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Coral);

    // helps to get the lines a bit sharper when the line width is not divisible by two.
    float offset = fmodf(lineWidth, 2.0f) / 2.0f;
    x+=offset;
    y+=offset;

    float hlw = lineWidth * 0.5f;

    // top line
    ctx.pushColorRect(x-hlw, y-hlw, width+lineWidth-1, lineWidth, strokeStyle.innerColor());

    // bottom line
    ctx.pushColorRect(x-hlw, y+height-hlw, width+lineWidth-1, lineWidth, strokeStyle.innerColor());

    // left line
    ctx.pushColorRect(x-hlw, y+hlw, lineWidth, height-lineWidth, strokeStyle.innerColor());

    // right line
    ctx.pushColorRect(x+width-hlw-1, y+hlw, lineWidth, height-lineWidth, strokeStyle.innerColor());
}

inline void Context::clearRect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.pushColorRect(std::move(x), std::move(y), std::move(width), std::move(height), detail::gfxStates.backgroundColor);
}

inline void Context::beginPath()
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.reset();
}

inline void Context::closePath()
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.closePath();
}

inline void Context::moveTo(float x, float y)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.moveTo(std::move(x), std::move(y));
}

inline void Context::lineTo(float x, float y)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.lineTo(std::move(x), std::move(y));
}

inline void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.bezierCurveTo(std::move(cp1x), std::move(cp1y), std::move(cp2x), std::move(cp2y), std::move(x), std::move(y));
}

inline void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.quadraticCurveTo(std::move(cpx), std::move(cpy), std::move(x), std::move(y));
}

inline void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.arc(std::move(x), std::move(y), std::move(radius), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.arcTo(std::move(x1), std::move(y1), std::move(x2), std::move(y2), std::move(radius));
}

inline void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.ellipse(std::move(x), std::move(y), std::move(radiusX), std::move(radiusY), std::move(rotation), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::rect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.currentPath.rect(std::move(x), std::move(y), std::move(width), std::move(height));
}

inline void Context::fill(Fill fillRule)
{
    EASY_FUNCTION(profiler::colors::Coral);
    fill(ctx.currentPath, std::move(fillRule));
}

inline void Context::stroke()
{
    EASY_FUNCTION(profiler::colors::Coral);
    stroke(ctx.currentPath);
}

inline void Context::fill(Path2D &path, Fill fillRule)
{
    EASY_FUNCTION(profiler::colors::Coral);
    ctx.fill(path, fillRule, fillStyle);
}

inline void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::Coral);
    //ctx.stroke(path, strokeStyle, lineJoin, miterLimit);
}


}

