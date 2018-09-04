#include <Tunis.h>

#include <easy/profiler.h>
namespace tunis
{

inline void Context::clearFrame(int fbLeft, int fbTop, int fbWidth, int fbHeight, Color backgroundColor)
{
    EASY_FUNCTION(profiler::colors::Teal);

    // update the clear color if necessary
    if (detail::global.backgroundColor != backgroundColor)
    {
        detail::global.backgroundColor = backgroundColor;

        glClearColor(backgroundColor.r/255.0f,
                     backgroundColor.g/255.0f,
                     backgroundColor.b/255.0f,
                     backgroundColor.a/255.0f);
    }

    // update the viewport if necessary
    Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
    if (detail::global.viewport != viewport)
    {
        detail::global.viewport = viewport;

        glViewport(fbLeft, fbTop, fbWidth, fbHeight);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


inline void Context::beginFrame(int winWidth, int winHeight, float devicePixelRatio)
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgBeginFrame(ctx.nvg,
                  static_cast<float>(winWidth),
                  static_cast<float>(winHeight),
                  devicePixelRatio);
}

inline void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgEndFrame(ctx.nvg);
}

inline void Context::fillRect(float x, float y, float width, float height)
{
    beginPath();
    rect(x, y, width, height);
    fill(ctx.currentPath);
}

inline void Context::strokeRect(float x, float y, float width, float height)
{
    beginPath();
    rect(x, y, width, height);
    stroke(ctx.currentPath);
}

inline void Context::clearRect(float x, float y, float width, float height)
{
    nvgBeginPath(ctx.nvg);
    nvgRect(ctx.nvg, x, y, width, height);
    nvgFillColor(ctx.nvg, nvgRGBA(detail::global.backgroundColor.r,
                                   detail::global.backgroundColor.g,
                                   detail::global.backgroundColor.b,
                                   detail::global.backgroundColor.a));
    nvgPathWinding(ctx.nvg, NVG_SOLID);
    nvgFill(ctx.nvg);
}

inline void Context::beginPath()
{
    ctx.currentPath.reset();
}

inline void Context::closePath()
{
    ctx.currentPath.closePath();
}

inline void Context::moveTo(float x, float y)
{
    ctx.currentPath.moveTo(std::move(x), std::move(y));
}

inline void Context::lineTo(float x, float y)
{
    ctx.currentPath.lineTo(std::move(x), std::move(y));
}

inline void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    ctx.currentPath.bezierCurveTo(std::move(cp1x), std::move(cp1y), std::move(cp2x), std::move(cp2y), std::move(x), std::move(y));
}

inline void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    ctx.currentPath.quadraticCurveTo(std::move(cpx), std::move(cpy), std::move(x), std::move(y));
}

inline void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    ctx.currentPath.arc(std::move(x), std::move(y), std::move(radius), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    ctx.currentPath.arcTo(std::move(x1), std::move(y1), std::move(x2), std::move(y2), std::move(radius));
}

inline void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    ctx.currentPath.ellipse(std::move(x), std::move(y), std::move(radiusX), std::move(radiusY), std::move(rotation), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::rect(float x, float y, float width, float height)
{
    ctx.currentPath.rect(std::move(x), std::move(y), std::move(width), std::move(height));
}

inline void Context::fill(Fill fillRule)
{
    fill(ctx.currentPath, std::move(fillRule));
}

inline void Context::stroke()
{
    stroke(ctx.currentPath);
}

inline void Context::fill(Path2D &path, Fill fillRule)
{
    EASY_FUNCTION(profiler::colors::Teal);

    ctx.pathToNVG(path);

    Color color = fillStyle.innerColor();
    nvgFillColor(ctx.nvg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgPathWinding(ctx.nvg, fillRule == FillNonZero ? NVG_SOLID : NVG_HOLE);
    nvgFill(ctx.nvg);
}


inline void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::Teal);

    ctx.pathToNVG(path);

    Color color = strokeStyle.innerColor();
    nvgStrokeColor(ctx.nvg, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgStrokeWidth(ctx.nvg, lineWidth);
    nvgStroke(ctx.nvg);
}

namespace detail
{
inline void ContextData::pathToNVG(Path2D &path)
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
}
}

}
