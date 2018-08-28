#include <Tunis.h>

namespace tunis
{

inline void Context::fillRect(float x, float y, float width, float height)
{
    beginPath();
    rect(x, y, width, height);
    fill(m_currentPath);
}

inline void Context::strokeRect(float x, float y, float width, float height)
{
    beginPath();
    rect(x, y, width, height);
    stroke(m_currentPath);
}

inline void Context::beginPath()
{
    m_currentPath.reset();
}

inline void Context::closePath()
{
    m_currentPath.closePath();
}

inline void Context::moveTo(float x, float y)
{
    m_currentPath.moveTo(x, y);
}

inline void Context::lineTo(float x, float y)
{
    m_currentPath.lineTo(x, y);
}

inline void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    m_currentPath.bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
}

inline void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    m_currentPath.quadraticCurveTo(cpx, cpy, x, y);
}

inline void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    m_currentPath.arc(x, y, radius, startAngle, endAngle, anticlockwise);
}

inline void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    m_currentPath.arcTo(x1, y1, x2, y2, radius);
}

inline void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    m_currentPath.ellipse(x, y, radiusX, radiusY, rotation, startAngle, endAngle, anticlockwise);
}

inline void Context::rect(float x, float y, float width, float height)
{
    m_currentPath.rect(x, y, width, height);
}

inline void Context::fill(FillRule fillRule)
{
    fill(m_currentPath, fillRule);
}

inline void Context::stroke()
{
    stroke(m_currentPath);
}

}
