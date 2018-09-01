#include <TunisPath2D.h>

#include <glm/common.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

namespace tunis
{

inline Path2D::Path2D()
{
    reset();
}

inline void Path2D::reset()
{
    commands().resize(0);
    points().resize(0);
    dirty() = false;
}

inline void Path2D::closePath()
{
    commands().push(detail::CLOSE, 0, 0, 0, 0, 0, 0, 0, 0);
    dirty() = true;
}

inline void Path2D::moveTo(float x, float y)
{
    commands().push(detail::MOVE_TO, std::move(x), std::move(y),
                    0, 0, 0, 0, 0, 0);
    dirty() = true;
}

inline void Path2D::lineTo(float x, float y)
{
    commands().push(detail::LINE_TO, std::move(x), std::move(y),
                    0, 0, 0, 0, 0, 0);
    dirty() = true;
}

inline void Path2D::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    commands().push(detail::BEZIER_TO,
                    std::move(cp1x), std::move(cp1y),
                    std::move(cp2x), std::move(cp2y),
                    std::move(x), std::move(y), 0, 0);
    dirty() = true;
}

inline void Path2D::quadraticCurveTo(float cx, float cy, float x, float y)
{
    commands().push(detail::QUAD_TO,
                    std::move(cx), std::move(cy),
                    std::move(x), std::move(y),
                    0, 0, 0, 0);
    dirty() = true;
}

inline void Path2D::arc(float cx, float cy, float r, float a0, float a1, bool anticlockwise)
{
    commands().push(detail::ARC,
                    std::move(cx), std::move(cy),
                    std::move(r),
                    std::move(a0), std::move(a1),
                    anticlockwise?1.f:0.f,
                    0, 0);
    dirty() = true;
}

inline void Path2D::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    commands().push(detail::ARC_TO,
                    std::move(x1), std::move(y1),
                    std::move(x2), std::move(y2),
                    std::move(radius),
                    0, 0, 0);
    dirty() = true;
}

inline void Path2D::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    commands().push(detail::ELLIPSE,
                    std::move(x), std::move(y),
                    std::move(radiusX), std::move(radiusY),
                    std::move(rotation),
                    std::move(startAngle), std::move(endAngle),
                    anticlockwise?1.f:0.f);
    dirty() = true;
}

inline void Path2D::rect(float x, float y, float width, float height)
{
    commands().push(detail::RECT,
                    std::move(x), std::move(y),
                    std::move(width), std::move(height),
                    0, 0, 0, 0);
    dirty() = true;
}

}
