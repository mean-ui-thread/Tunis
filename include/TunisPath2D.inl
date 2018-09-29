#include <TunisPath2D.h>

#include <cfloat>

namespace tunis
{

inline Path2D::Path2D()
{
    reset();
}

inline void Path2D::reset()
{
    commands().resize(0);
    subPathCount() = 0;
    dirty() = false;
    boundTopLeft() = glm::vec2(FLT_MAX);
    boundBottomRight() = glm::vec2(-FLT_MAX);
}

inline void Path2D::closePath()
{
    commands().push(detail::PathCommandType::close, 0, 0, 0, 0, 0, 0, 0, 0);
    dirty() = true;
}

inline void Path2D::moveTo(float x, float y)
{
    commands().push(detail::PathCommandType::moveTo, std::move(x), std::move(y),
                    0, 0, 0, 0, 0, 0);
    dirty() = true;
}

inline void Path2D::lineTo(float x, float y)
{
    commands().push(detail::PathCommandType::lineTo, std::move(x), std::move(y),
                    0, 0, 0, 0, 0, 0);
    dirty() = true;
}

inline void Path2D::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    commands().push(detail::PathCommandType::bezierCurveTo,
                    std::move(cp1x), std::move(cp1y),
                    std::move(cp2x), std::move(cp2y),
                    std::move(x), std::move(y), 0, 0);
    dirty() = true;
}

inline void Path2D::quadraticCurveTo(float cx, float cy, float x, float y)
{
    commands().push(detail::PathCommandType::quadraticCurveTo,
                    std::move(cx), std::move(cy),
                    std::move(x), std::move(y),
                    0, 0, 0, 0);
    dirty() = true;
}

inline void Path2D::arc(float cx, float cy, float r, float a0, float a1, bool anticlockwise)
{
    commands().push(detail::PathCommandType::arc,
                    std::move(cx), std::move(cy),
                    std::move(r),
                    std::move(a0), std::move(a1),
                    anticlockwise?1.f:0.f,
                    0, 0);
    dirty() = true;
}

inline void Path2D::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    commands().push(detail::PathCommandType::arcTo,
                    std::move(x1), std::move(y1),
                    std::move(x2), std::move(y2),
                    std::move(radius),
                    0, 0, 0);
    dirty() = true;
}

inline void Path2D::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    commands().push(detail::PathCommandType::ellipse,
                    std::move(x), std::move(y),
                    std::move(radiusX), std::move(radiusY),
                    std::move(rotation),
                    std::move(startAngle), std::move(endAngle),
                    anticlockwise?1.f:0.f);
    dirty() = true;
}

inline void Path2D::rect(float x, float y, float width, float height)
{
    commands().push(detail::PathCommandType::rect,
                    std::move(x), std::move(y),
                    std::move(width), std::move(height),
                    0, 0, 0, 0);
    dirty() = true;
}

}
