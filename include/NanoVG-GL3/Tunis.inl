#include <Tunis.h>

#include <easy/profiler.h>
namespace tunis
{

inline void Context::clearFrame(int fbLeft, int fbTop, int fbWidth, int fbHeight, Color backgroundColor)
{
    EASY_FUNCTION(profiler::colors::Teal);

    // update the clear color if necessary
    if (detail::globalContextData.backgroundColor != backgroundColor)
    {
        detail::globalContextData.backgroundColor = backgroundColor;

        glClearColor(backgroundColor.r/255.0f,
                     backgroundColor.g/255.0f,
                     backgroundColor.b/255.0f,
                     backgroundColor.a/255.0f);
    }

    // update the viewport if necessary
    Viewport viewport(fbLeft, fbTop, fbWidth, fbHeight);
    if (detail::globalContextData.viewport != viewport)
    {
        detail::globalContextData.viewport = viewport;

        glViewport(fbLeft, fbTop, fbWidth, fbHeight);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}


inline void Context::beginFrame(int winWidth, int winHeight, double devicePixelRatio)
{
    nvgBeginFrame(data.ctx,
                  static_cast<double>(winWidth),
                  static_cast<double>(winHeight),
                  devicePixelRatio);
}

inline void Context::endFrame()
{
    EASY_FUNCTION(profiler::colors::Teal);
    nvgEndFrame(data.ctx);
}

inline void Context::fillRect(double x, double y, double width, double height)
{
    beginPath();
    rect(x, y, width, height);
    fill(data.currentPath);
}

inline void Context::strokeRect(double x, double y, double width, double height)
{
    beginPath();
    rect(x, y, width, height);
    stroke(data.currentPath);
}

inline void Context::clearRect(double x, double y, double width, double height)
{
    nvgBeginPath(data.ctx);
    nvgRect(data.ctx, x, y, width, height);
    nvgFillColor(data.ctx, nvgRGBA(detail::globalContextData.backgroundColor.r,
                                   detail::globalContextData.backgroundColor.g,
                                   detail::globalContextData.backgroundColor.b,
                                   detail::globalContextData.backgroundColor.a));
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

inline void Context::moveTo(double x, double y)
{
    data.currentPath.moveTo(x, y);
}

inline void Context::lineTo(double x, double y)
{
    data.currentPath.lineTo(x, y);
}

inline void Context::bezierCurveTo(double cp1x, double cp1y, double cp2x, double cp2y, double x, double y)
{
    data.currentPath.bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}

inline void Context::quadraticCurveTo(double cpx, double cpy, double x, double y)
{
    data.currentPath.quadraticCurveTo(cpx, cpy, x, y);
}

inline void Context::arc(double x, double y, double radius, double startAngle, double endAngle, bool anticlockwise)
{
    data.currentPath.arc(x, y, radius, startAngle, endAngle, anticlockwise);
}

inline void Context::arcTo(double x1, double y1, double x2, double y2, double radius)
{
    data.currentPath.arcTo(x1, y1, x2, y2, radius);
}

inline void Context::ellipse(double x, double y, double radiusX, double radiusY, double rotation, double startAngle, double endAngle, bool anticlockwise)
{
    data.currentPath.ellipse(x, y, radiusX, radiusY, rotation, startAngle, endAngle, anticlockwise);
}

inline void Context::rect(double x, double y, double width, double height)
{
    data.currentPath.rect(x, y, width, height);
}

inline void Context::fill(FillRule fillRule)
{
    fill(data.currentPath, fillRule);
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

    size_t pointId = 0;
    for(auto cmd : path.commands())
    {
        switch(cmd)
        {
        case detail::CLOSE:
            nvgClosePath(ctx);
            break;
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
                        path.points()[pointId].z, path.points()[pointId+1].x,
                        path.points()[pointId+1].y, path.points()[pointId+1].z);
            pointId+=2;
            break;
        case detail::QUAD_TO:
            nvgQuadTo(ctx,
                      path.points()[pointId].x,   path.points()[pointId].y,
                      path.points()[pointId+1].x, path.points()[pointId+1].y);
            pointId+=2;
            break;
        case detail::ARC:
            nvgArc(ctx,
                   path.points()[pointId].x,   path.points()[pointId].y,
                   path.points()[pointId].z,   path.points()[pointId+1].x,
                   path.points()[pointId+1].y, path.points()[pointId+1].z > 0.5 ? NVG_CCW : NVG_CW);
            pointId+=2;
            break;
        case detail::ARC_TO:
            nvgArcTo(ctx,
                   path.points()[pointId].x,   path.points()[pointId].y,
                   path.points()[pointId].z,   path.points()[pointId+1].x,
                   path.points()[pointId+1].y);
            pointId+=2;
            break;
        case detail::ELLIPSE:
            // TODO figure out how to use rotation, startAngle, endAngle, anticlockwise here...
            nvgEllipse(ctx,
                       path.points()[pointId].x,   path.points()[pointId].y,
                       path.points()[pointId].z,   path.points()[pointId+1].x);
            pointId+=3;
            break;
        case detail::RECT:
            nvgRect(ctx,
                    path.points()[pointId].x,   path.points()[pointId].y,
                    path.points()[pointId+1].x,   path.points()[pointId+1].y);
            pointId+=2;
            break;
        }
    }
}
}

}
