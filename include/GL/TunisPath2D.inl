#include <TunisPath2D.h>

#include <glm/common.hpp>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>

#ifndef TUNIS_DIST_TOL
#define TUNIS_DIST_TOL 0.01
#endif

namespace tunis
{

namespace detail {

static double normalize(double *x, double* y)
{
    double d = glm::sqrt((*x)*(*x) + (*y)*(*y));
    if (d > 1e-6)
    {
        double id = 1.0 / d;
        *x *= id;
        *y *= id;
    }
    return d;
}

static double cross(double dx0, double dy0, double dx1, double dy1)
{
    return dx1*dy0 - dx0*dy1;
}


static bool ptEquals(double x1, double y1, double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    return dx*dx + dy*dy < TUNIS_DIST_TOL * TUNIS_DIST_TOL;
}

static double distPtSeg(double x, double y, double px, double py, double qx, double qy)
{
    double pqx, pqy, dx, dy, d, t;
    pqx = qx-px;
    pqy = qy-py;
    dx = x-px;
    dy = y-py;
    d = pqx*pqx + pqy*pqy;
    t = pqx*dx + pqy*dy;
    if (d > 0) t /= d;
    if (t < 0) t = 0;
    else if (t > 1) t = 1;
    dx = px + t*pqx - x;
    dy = py + t*pqy - y;
    return dx*dx + dy*dy;
}

static double calcCubicBezier1d(double x0, double x1, double x2, double x3, double t )
{
    // see openvg 1.0 spec section 8.3.2 Cubic Bezier Curves
    double oneT = 1.0 - t;
    double x =		x0 * (oneT * oneT * oneT)
    +	3.0 * x1 * (oneT * oneT) * t
    +	3.0 * x2 * oneT * (t * t)
    +	x3 * (t * t * t);
    return x;
}
}


inline Path2D::Path2D()
{
    reset();
}

inline void Path2D::reset()
{
    commands().resize(0);
    points().resize(0);
    vertexCache().resize(0);
    minBoundX() = DBL_MAX;
    minBoundY() = DBL_MAX;
    maxBoundX() = -DBL_MAX;
    maxBoundY() = -DBL_MAX;
}

inline void Path2D::closePath()
{
    commands().push_back(detail::CLOSE);
    vertexCache().resize(0);
}

inline void Path2D::moveTo(double x, double y)
{
    commands().push_back(detail::MOVE_TO);
    points().push_back(Point(x, y, 0));
    minBoundX() = glm::min(minBoundX(), x);
    minBoundY() = glm::min(minBoundY(), y);
    maxBoundX() = glm::max(maxBoundX(), x);
    maxBoundY() = glm::max(maxBoundY(), y);
    vertexCache().resize(0);
}

inline void Path2D::lineTo(double x, double y)
{
    commands().push_back(detail::LINE_TO);
    points().push_back(Point(x, y, 0));
    minBoundX() = glm::min(minBoundX(), x);
    minBoundY() = glm::min(minBoundY(), y);
    maxBoundX() = glm::max(maxBoundX(), x);
    maxBoundY() = glm::max(maxBoundY(), y);
    vertexCache().resize(0);
}

inline void Path2D::bezierCurveTo(double cp1x, double cp1y, double cp2x, double cp2y, double x, double y)
{
    Point start = points().size() == 0 ? Point(0) : points().back();
    const double increment = 1.0 / 16.0;
    for (double t = increment; t < 1.0 + increment; t+=increment )
    {
        lineTo(detail::calcCubicBezier1d( start.x, cp1x, cp2x, x, t ),
               detail::calcCubicBezier1d( start.y, cp1y, cp2y, y, t ));
    }
}

inline void Path2D::quadraticCurveTo(double cx, double cy, double x, double y)
{
    Point p = points().size() == 0 ? Point(0) : points().back();
    bezierCurveTo(p.x + 2.0/3.0*(cx - p.x), p.y + 2.0/3.0*(cy - p.y),
                  x + 2.0/3.0*(cx - x), y + 2.0/3.0*(cy - y),
                  x, y);
}

inline void Path2D::arc(double cx, double cy, double r, double a0, double a1, bool anticlockwise)
{
    // Clamp angles
    double da = a1 - a0;
    if (anticlockwise)
    {
        if (glm::abs(da) >= glm::two_pi<double>())
        {
            da = -glm::two_pi<double>();
        }
        else
        {
            while (da > 0.0) da -= glm::two_pi<double>();
        }
    }
    else
    {
        if (glm::abs(da) >= glm::two_pi<double>())
        {
            da = glm::two_pi<double>();
        }
        else
        {
            while (da < 0.0) da += glm::two_pi<double>();
        }
    }

    // Split arc into max 90 degree segments.
    int ndivs = static_cast<int>(glm::max(1.0, glm::min((glm::abs(da) / (glm::half_pi<double>()) + 0.5), 5.0)));
    double hda = (da / static_cast<double>(ndivs)) / 2.0;
    double kappa = glm::abs(4.0 / 3.0 * (1.0 - glm::cos(hda)) / glm::sin(hda));

    if (anticlockwise)
    {
        kappa = -kappa;
    }

    double px = 0, py = 0, ptanx = 0, ptany = 0;

    for (int i = 0; i <= ndivs; ++i) {
        double a = a0 + da * (i/static_cast<double>(ndivs));
        double dx = glm::cos(a);
        double dy = glm::sin(a);
        double x = cx + dx * r;
        double y = cy + dy * r;
        double tanx = -dy * r * kappa;
        double tany = dx * r * kappa;

        if (i == 0)
        {
            if (commands().size() == 0)
            {
                moveTo(x, y);
            }
            else
            {
                lineTo(x, y);
            }
        }
        else
        {
            bezierCurveTo(px+ptanx, py+ptany,
                          x-tanx, y-tany,
                          x, y);
        }
        px = x;
        py = y;
        ptanx = tanx;
        ptany = tany;
    }
}

inline void Path2D::arcTo(double x1, double y1, double x2, double y2, double radius)
{
    if (commands().size() == 0)
    {
        return;
    }

    Point p = points().size() == 0 ? Point(0) : points().back();

    double x0 = p.x;
    double y0 = p.y;
    double dx0,dy0, dx1,dy1, a, d, cx,cy, a0,a1;
    bool anticlockwize;

    // Handle degenerate cases.
    if (detail::ptEquals(x0,y0, x1,y1) ||
        detail::ptEquals(x1,y1, x2,y2) ||
        detail::distPtSeg(x1,y1, x0,y0, x2,y2) < TUNIS_DIST_TOL * TUNIS_DIST_TOL ||
        radius < TUNIS_DIST_TOL)
    {
        lineTo(x1,y1);
        return;
    }

    // Calculate tangential circle to lines (x0,y0)-(x1,y1) and (x1,y1)-(x2,y2).
    dx0 = x0-x1;
    dy0 = y0-y1;
    dx1 = x2-x1;
    dy1 = y2-y1;
    detail::normalize(&dx0,&dy0);
    detail::normalize(&dx1,&dy1);
    a = glm::acos(dx0*dx1 + dy0*dy1);
    d = radius / glm::tan(a/2.0);

    if (d > 10000.0)
    {
        lineTo(x1, y1);
        return;
    }

    if (detail::cross(dx0,dy0, dx1,dy1) > 0.0)
    {
        cx = x1 + dx0*d + dy0*radius;
        cy = y1 + dy0*d + -dx0*radius;
        a0 = std::atan2(dx0, -dy0);
        a1 = std::atan2(-dx1, dy1);
        anticlockwize = false;
    }
    else
    {
        cx = x1 + dx0*d + -dy0*radius;
        cy = y1 + dy0*d + dx0*radius;
        a0 = std::atan2(-dx0, dy0);
        a1 = std::atan2(dx1, -dy1);
        anticlockwize = true;
    }

    arc(cx, cy, radius, a0, a1, anticlockwize);

}

inline void Path2D::ellipse(double x, double y, double radiusX, double radiusY, double rotation, double startAngle, double endAngle, bool anticlockwise)
{
    // TODO
}

inline void Path2D::rect(double x, double y, double width, double height)
{
    moveTo(x,         y);
    lineTo(x,         y + height);
    lineTo(x,         y + height);
    lineTo(x + width, y + height);
    lineTo(x + width, y);
    closePath();
}

}
