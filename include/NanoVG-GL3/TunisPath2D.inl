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
}

inline void Path2D::closePath()
{
    commands().push_back(detail::CLOSE);
}

inline void Path2D::moveTo(double x, double y)
{
    commands().push_back(detail::MOVE_TO);
    points().push_back(Point(x, y, 0));
}

inline void Path2D::lineTo(double x, double y)
{
    commands().push_back(detail::LINE_TO);
    points().push_back(Point(x, y, 0));
}

inline void Path2D::bezierCurveTo(double cp1x, double cp1y, double cp2x, double cp2y, double x, double y)
{
    commands().push_back(detail::BEZIER_TO);
    points().push_back(Point(cp1x, cp1y, cp2x));
    points().push_back(Point(cp2y, x, y));
}

inline void Path2D::quadraticCurveTo(double cx, double cy, double x, double y)
{
    commands().push_back(detail::QUAD_TO);
    points().push_back(Point(cx, cy, 0));
    points().push_back(Point(x, y, 0));
}

inline void Path2D::arc(double cx, double cy, double r, double a0, double a1, bool anticlockwise)
{
    commands().push_back(detail::ARC);
    points().push_back(Point(cx, cy, r));
    points().push_back(Point(a0, a1, anticlockwise ? 1.0 : 0.0));
}

inline void Path2D::arcTo(double x1, double y1, double x2, double y2, double radius)
{
    commands().push_back(detail::ARC_TO);
    points().push_back(Point(x1, y1, x2));
    points().push_back(Point(y2, radius, 0));
}

inline void Path2D::ellipse(double x, double y, double radiusX, double radiusY, double rotation, double startAngle, double endAngle, bool anticlockwise)
{
    commands().push_back(detail::ELLIPSE);
    points().push_back(Point(x, y, radiusX));
    points().push_back(Point(radiusY, rotation, startAngle));
    points().push_back(Point(endAngle, anticlockwise ? 1.0 : 0.0, 0));
}

inline void Path2D::rect(double x, double y, double width, double height)
{
    commands().push_back(detail::RECT);
    points().push_back(Point(x, y, 0));
    points().push_back(Point(width, height, 0));
}

}
