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
    nvgBeginFrame(data.ctx,
                  static_cast<float>(winWidth),
                  static_cast<float>(winHeight),
                  devicePixelRatio);
}

inline void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgEndFrame(data.ctx);
}

inline void Context::fillRect(float x, float y, float width, float height)
{
    beginPath();
    rect(x, y, width, height);
    fill(data.currentPath);
}

inline void Context::strokeRect(float x, float y, float width, float height)
{
    beginPath();
    rect(x, y, width, height);
    stroke(data.currentPath);
}

inline void Context::clearRect(float x, float y, float width, float height)
{
    nvgBeginPath(data.ctx);
    nvgRect(data.ctx, x, y, width, height);
    nvgFillColor(data.ctx, nvgRGBA(detail::global.backgroundColor.r,
                                   detail::global.backgroundColor.g,
                                   detail::global.backgroundColor.b,
                                   detail::global.backgroundColor.a));
    nvgPathWinding(data.ctx, NVG_SOLID);
    nvgFill(data.ctx);
}

inline void Context::beginPath()
{
    data.currentPath.reset();
}

inline void Context::closePath()
{
    data.currentPath.closePath();
}

inline void Context::moveTo(float x, float y)
{
    data.currentPath.moveTo(std::move(x), std::move(y));
}

inline void Context::lineTo(float x, float y)
{
    data.currentPath.lineTo(std::move(x), std::move(y));
}

inline void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    data.currentPath.bezierCurveTo(std::move(cp1x), std::move(cp1y), std::move(cp2x), std::move(cp2y), std::move(x), std::move(y));
}

inline void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    data.currentPath.quadraticCurveTo(std::move(cpx), std::move(cpy), std::move(x), std::move(y));
}

inline void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    data.currentPath.arc(std::move(x), std::move(y), std::move(radius), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    data.currentPath.arcTo(std::move(x1), std::move(y1), std::move(x2), std::move(y2), std::move(radius));
}

inline void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    data.currentPath.ellipse(std::move(x), std::move(y), std::move(radiusX), std::move(radiusY), std::move(rotation), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::rect(float x, float y, float width, float height)
{
    data.currentPath.rect(std::move(x), std::move(y), std::move(width), std::move(height));
}

inline void Context::fill(FillRule fillRule)
{
    fill(data.currentPath, std::move(fillRule));
}

inline void Context::stroke()
{
    stroke(data.currentPath);
}

inline void Context::fill(Path2D &path, FillRule fillRule)
{
    EASY_FUNCTION(profiler::colors::Teal);

    data.pathToNVG(path);

    Color color = fillStyle.innerColor();
    nvgFillColor(data.ctx, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgPathWinding(data.ctx, fillRule == nonzero ? NVG_SOLID : NVG_HOLE);
    nvgFill(data.ctx);
}


inline void Context::stroke(Path2D &path)
{
    EASY_FUNCTION(profiler::colors::Teal);

    data.pathToNVG(path);

    Color color = strokeStyle.innerColor();
    nvgStrokeColor(data.ctx, nvgRGBA(color.r, color.g, color.b, color.a));
    nvgStrokeWidth(data.ctx, lineWidth);
    nvgStroke(data.ctx);
}

namespace detail
{
inline void ContextData::pathToNVG(Path2D &path)
{
    nvgBeginPath(ctx);

    for(size_t i = 0; i < path.commands().size(); ++i)
    {
        switch(path.commands().type(i))
        {
        case detail::CLOSE:
            nvgClosePath(ctx);
            break;
        case detail::MOVE_TO:
            nvgMoveTo(ctx, path.commands().param0(i), path.commands().param1(i));
            break;
        case detail::LINE_TO:
            nvgLineTo(ctx, path.commands().param0(i), path.commands().param1(i));
            break;
        case detail::BEZIER_TO:
            nvgBezierTo(ctx,
                        path.commands().param0(i), path.commands().param1(i),
                        path.commands().param2(i), path.commands().param3(i),
                        path.commands().param4(i), path.commands().param5(i));
            break;
        case detail::QUAD_TO:
            nvgQuadTo(ctx,
                      path.commands().param0(i), path.commands().param1(i),
                      path.commands().param2(i), path.commands().param3(i));
            break;
        case detail::ARC:
            nvgArc(ctx,
                   path.commands().param0(i), path.commands().param1(i),
                   path.commands().param2(i), path.commands().param3(i),
                   path.commands().param4(i), path.commands().param5(i) > 0.5f ? NVG_CCW : NVG_CW);
            break;
        case detail::ARC_TO:
            nvgArcTo(ctx,
                     path.commands().param0(i), path.commands().param1(i),
                     path.commands().param2(i), path.commands().param3(i),
                     path.commands().param4(i));
            break;
        case detail::ELLIPSE:
            // TODO figure out how to use rotation, startAngle, endAngle, anticlockwise here...
            nvgEllipse(ctx,
                       path.commands().param0(i), path.commands().param1(i),
                       path.commands().param2(i), path.commands().param3(i));
            break;
        case detail::RECT:
            nvgRect(ctx,
                    path.commands().param0(i), path.commands().param1(i),
                    path.commands().param2(i), path.commands().param3(i));
            break;
        }
    }
}
}

}
