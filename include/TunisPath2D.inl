#include <TunisPath2D.h>

#include <glm/common.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

namespace tunis
{

namespace detail {
static float distPtSeg(Point p0, Point p1, Point p2)
{
    Point d0 = p2 - p1;
    Point d1 = p0 - p1;
    float d = d0.x * d0.x + d0.y * d0.y;
    float t = d0.x * d1.x + d0.y * d1.y;
    if (d > 0) t /= d;
    if (t < 0) t = 0;
    else if (t > 1) t = 1;
    d1 = p1 + t*d0 - p0;
    return d1.x*d1.x + d1.y*d1.y;
}
}


inline Path2D::Path2D()
{
    reset();
}

inline void Path2D::reset()
{
    commands().resize(1);
    commands().front() = detail::MOVE_TO;

    points().resize(1);
    points().front().x = 0.0f;
    points().front().y = 0.0f;
}

inline void Path2D::closePath()
{
    commands().push_back(detail::CLOSE);
}

inline void Path2D::moveTo(float x, float y)
{
    moveTo(Point(x, y));
}

inline void Path2D::moveTo(Point p)
{
    if (commands().back() == detail::MOVE_TO)
    {
        points().back() = p;
        return;
    }

    commands().push_back(detail::MOVE_TO);
    points().push_back(p);
}

inline void Path2D::lineTo(float x, float y)
{
    lineTo(Point(x, y));
}

inline void Path2D::lineTo(Point p)
{
    commands().push_back(detail::LINE_TO);
    points().push_back(p);
}


inline void Path2D::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    bezierCurveTo(Point(cp1x, cp1y), Point(cp2x, cp2y), Point(x, y));
}

inline void Path2D::bezierCurveTo(Point cp1, Point cp2, Point ep)
{
    commands().push_back(detail::BEZIER_TO);
    points().push_back(cp1);
    points().push_back(cp2);
    points().push_back(ep);
}

inline void Path2D::quadraticCurveTo(float cx, float cy, float x, float y)
{
    Point &p = points().back();
    bezierCurveTo(Point(p.x + 2.0f/3.0f*(cx - p.x), p.y + 2.0f/3.0f*(cy - p.y)),
                  Point(x + 2.0f/3.0f*(cx - x), y + 2.0f/3.0f*(cy - y)),
                  Point(x, y));
}

inline void Path2D::arc(float cx, float cy, float r, float a0, float a1, bool anticlockwise)
{
    const detail::CommandType move = commands().size() > 0 ? detail::LINE_TO : detail::MOVE_TO;

    // Clamp angles
    float da = a1 - a0;
    if (anticlockwise)
    {
        if (glm::abs(da) >= glm::two_pi<float>())
        {
            da = -glm::two_pi<float>();
        }
        else
        {
            while (da > 0.0f) da -= glm::two_pi<float>();
        }
    }
    else
    {
        if (glm::abs(da) >= glm::two_pi<float>())
        {
            da = glm::two_pi<float>();
        }
        else
        {
            while (da < 0.0f) da += glm::two_pi<float>();
        }
    }

    // Split arc into max 90 degree segments.
    int ndivs = static_cast<int>(glm::max(1.0f, glm::min((glm::abs(da) / (glm::half_pi<float>()) + 0.5f), 5.0f)));
    float hda = (da / static_cast<float>(ndivs)) / 2.0f;
    float kappa = glm::abs(4.0f / 3.0f * (1.0f - glm::cos(hda)) / glm::sin(hda));

    if (anticlockwise)
    {
        kappa = -kappa;
    }

    Point prevP(0);
    Point prevTan(0);

    for (int i = 0; i <= ndivs; ++i) {
        float a = a0 + da * (i/static_cast<float>(ndivs));
        float dx = glm::cos(a);
        float dy = glm::sin(a);
        Point p(cx + dx * r, cy + dy * r);
        Point tan(-dy * r * kappa, dx * r * kappa);

        if (i == 0)
        {
            commands().push_back(move);
            points().push_back(p);
        }
        else
        {
            commands().push_back(detail::BEZIER_TO);
            points().push_back(prevP + prevTan);
            points().push_back(p - tan);
            points().push_back(p);
        }
        prevP = p;
        prevTan = tan;
    }
}

inline void Path2D::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    if (commands().size() == 0)
    {
        return;
    }

    Point p0 = points().back();
    Point p1(x1, y1);
    Point p2(x2, y2);

    // Handle degenerate cases.
    if (glm::all(glm::epsilonEqual(p0, p1, glm::epsilon<float>())) ||
        glm::all(glm::epsilonEqual(p1, p2, glm::epsilon<float>())) ||
        detail::distPtSeg(p1, p0, p2) < glm::epsilon<float>()*glm::epsilon<float>() ||
        radius < glm::epsilon<float>()) {
        lineTo(p1);
        return;
    }

    // Calculate tangential circle to lines (x0,y0)-(x1,y1) and (x1,y1)-(x2,y2).
    Point d0 = glm::normalize(p0 - p1);
    Point d1 = glm::normalize(p2 - p1);
    float a = glm::acos(d0.x * d1.x + d0.y * d1.y);
    float d = radius / glm::tan(a/2.0f);

    if (d > 10000.0f)
    {
        lineTo(p1);
        return;
    }

    float cx,cy, a0, a1;
    bool anticlockwise;

    if ((d1.x * d0.y - d0.x * d1.y) > 0.0f)
    {
        cx = x1 + d0.x * d + d0.y * radius;
        cy = y1 + d0.y * d + -d0.x * radius;
        a0 = std::atan2(d0.x, -d0.y);
        a1 = std::atan2(-d1.x, d1.y);
        anticlockwise = false;
    }
    else
    {
        cx = x1 + d0.x * d + -d0.y * radius;
        cy = y1 + d0.y * d + d0.x * radius;
        a0 = std::atan2(-d0.x, d0.y);
        a1 = std::atan2(d1.x, -d1.y);
        anticlockwise = true;
    }

    arc(cx, cy, radius, a0, a1, anticlockwise);

}

inline void Path2D::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    // TODO
}

inline void Path2D::rect(float x, float y, float width, float height)
{
    moveTo(x,         y);
    lineTo(x,         y + height);
    lineTo(x,         y + height);
    lineTo(x + width, y + height);
    lineTo(x + width, y);
    closePath();
}

}
