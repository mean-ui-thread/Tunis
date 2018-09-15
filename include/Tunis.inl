#include <Tunis.h>
#include <easy/profiler.h>

namespace tunis
{

inline void Context::beginPath()
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.reset();
}

inline void Context::closePath()
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.closePath();
}

inline void Context::fillRect(float x, float y, float width, float height)
{
    rect(x, y, width, height);
    fill();
}

inline void Context::strokeRect(float x, float y, float width, float height)
{
    rect(x, y, width, height);
    stroke();
}

inline void Context::moveTo(float x, float y)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.moveTo(std::move(x), std::move(y));
}

inline void Context::lineTo(float x, float y)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.lineTo(std::move(x), std::move(y));
}

inline void Context::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.bezierCurveTo(std::move(cp1x), std::move(cp1y), std::move(cp2x), std::move(cp2y), std::move(x), std::move(y));
}

inline void Context::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.quadraticCurveTo(std::move(cpx), std::move(cpy), std::move(x), std::move(y));
}

inline void Context::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.arc(std::move(x), std::move(y), std::move(radius), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.arcTo(std::move(x1), std::move(y1), std::move(x2), std::move(y2), std::move(radius));
}

inline void Context::ellipse(float x, float y, float radiusX, float radiusY, float rotation, float startAngle, float endAngle, bool anticlockwise)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.ellipse(std::move(x), std::move(y), std::move(radiusX), std::move(radiusY), std::move(rotation), std::move(startAngle), std::move(endAngle), std::move(anticlockwise));
}

inline void Context::rect(float x, float y, float width, float height)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    currentPath.rect(std::move(x), std::move(y), std::move(width), std::move(height));
}

inline void Context::fill(FillRule fillRule)
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    fill(currentPath, std::move(fillRule));
}

inline void Context::stroke()
{
    EASY_FUNCTION(profiler::colors::DarkCyan)
    stroke(currentPath);
}


}

