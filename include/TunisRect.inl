#include <TunisRect.h>

#include <glm/gtc/epsilon.hpp>

namespace tunis
{

constexpr inline Rect::Rect() noexcept :
    m_x(.0f),
    m_y(.0f),
    m_width(.0f),
    m_height(.0f)
{
}

constexpr inline Rect::Rect(float aleft, float atop, float awidth, float aheight) noexcept :
    m_x(aleft),
    m_y(atop),
    m_width(awidth),
    m_height(aheight)
{
}

constexpr inline Rect::Rect(const Point &atopLeft, const Size &asize) noexcept :
    m_x(atopLeft.x),
    m_y(atopLeft.y),
    m_width(asize.width()),
    m_height(asize.height())
{
}

constexpr inline Rect::Rect(const Point &atopLeft, const Point &abottomRight) noexcept :
    m_x(atopLeft.x),
    m_y(atopLeft.y),
    m_width(abottomRight.x - atopLeft.x),
    m_height(abottomRight.y - atopLeft.y)
{
}

constexpr inline Rect::Rect(const Rect &r) noexcept :
    m_x(r.m_x),
    m_y(r.m_y),
    m_width(r.m_width),
    m_height(r.m_height)
{
}

inline bool Rect::isNull() const noexcept
{
    return glm::epsilonEqual(m_width, 0.0f, glm::epsilon<float>()) &&
            glm::epsilonEqual(m_height, 0.0f, glm::epsilon<float>());
}
constexpr inline bool Rect::isEmpty() const noexcept
{
    return m_width <= .0f || m_height <= .0f;
}

constexpr inline bool Rect::isValid() const noexcept
{
    return m_width > .0f && m_height > .0f;
}

constexpr float Rect::left() const noexcept
{
    return m_x;
}

constexpr float Rect::top() const noexcept
{
    return m_y;
}

constexpr float Rect::right() const noexcept
{
    return m_x + m_width;
}

constexpr float Rect::bottom() const noexcept
{
    return m_y + m_height;
}

constexpr inline float &Rect::x() noexcept
{
    return m_x;
}

constexpr inline float Rect::x() const noexcept
{
    return m_x;
}

constexpr inline float &Rect::y() noexcept
{
    return m_y;
}

constexpr inline float Rect::y() const noexcept
{
    return m_y;
}

constexpr inline void Rect::setLeft(float pos) noexcept
{
    float diff = pos - m_x;
    m_x += diff;
    m_width -= diff;
}

constexpr inline void Rect::setRight(float pos) noexcept
{
    m_width = pos - m_x;
}

constexpr inline void Rect::setTop(float pos) noexcept
{
    float diff = pos - m_y;
    m_y += diff;
    m_height -= diff;
}

constexpr inline void Rect::setBottom(float pos) noexcept
{
    m_height = pos - m_y;
}

constexpr inline void Rect::setTopLeft(const Point &p) noexcept
{
    setLeft(p.x);
    setTop(p.y);
}

constexpr inline void Rect::setTopRight(const Point &p) noexcept
{
    setRight(p.x);
    setTop(p.y);
}

constexpr inline void Rect::setBottomLeft(const Point &p) noexcept
{
    setLeft(p.x);
    setBottom(p.y);
}

constexpr inline void Rect::setBottomRight(const Point &p) noexcept
{
    setRight(p.x);
    setBottom(p.y);
}

constexpr inline Point Rect::center() const noexcept
{
    return Point(m_x + m_width/2, m_y + m_height/2);
}

constexpr inline void Rect::moveLeft(float pos) noexcept
{
    m_x = pos;
}

constexpr inline void Rect::moveTop(float pos) noexcept
{
    m_y = pos;
}

constexpr inline void Rect::moveRight(float pos) noexcept
{
    m_x = pos - m_width;
}

constexpr inline void Rect::moveBottom(float pos) noexcept
{
    m_y = pos - m_height;
}

constexpr inline void Rect::moveTopLeft(const Point &p) noexcept
{
    moveLeft(p.x);
    moveTop(p.y);
}

constexpr inline void Rect::moveTopRight(const Point &p) noexcept
{
    moveRight(p.x);
    moveTop(p.y);
}

constexpr inline void Rect::moveBottomLeft(const Point &p) noexcept
{
    moveLeft(p.x);
    moveBottom(p.y);
}

constexpr inline void Rect::moveBottomRight(const Point &p) noexcept
{
    moveRight(p.x);
    moveBottom(p.y);
}

constexpr inline void Rect::moveCenter(const Point &p) noexcept
{
    m_x = p.x - m_width/2;
    m_y = p.y - m_height/2;
}
constexpr inline float Rect::width() const noexcept
{
    return m_width;
}

constexpr inline float Rect::height() const noexcept
{
    return m_height;
}

constexpr inline Size Rect::size() const noexcept
{
    return Size(m_width, m_height);
}

constexpr inline void Rect::translate(float dx, float dy) noexcept
{
    m_x += dx;
    m_y += dy;
}

constexpr inline void Rect::translate(const Point &p) noexcept
{
    m_x += p.x;
    m_y += p.y;
}

constexpr inline void Rect::moveTo(float ax, float ay) noexcept
{
    m_x = ax;
    m_y = ay;
}

constexpr inline void Rect::moveTo(const Point &p) noexcept
{
    m_x = p.x;
    m_y = p.y;
}

constexpr inline Rect Rect::translated(float dx, float dy) const noexcept
{
    return Rect(m_x + dx, m_y + dy, m_width, m_height);
}

constexpr inline Rect Rect::translated(const Point &p) const noexcept
{
    return Rect(m_x + p.x, m_y + p.y, m_width, m_height);
}

constexpr inline Rect Rect::transposed() const noexcept
{
    return Rect(topLeft(), size().transposed());
}

constexpr inline void Rect::getRect(float *ax, float *ay, float *aaw, float *aah) const noexcept
{
    *ax = this->m_x;
    *ay = this->m_y;
    *aaw = this->m_width;
    *aah = this->m_height;
}

constexpr inline void Rect::setRect(float ax, float ay, float aaw, float aah) noexcept
{
    this->m_x = ax;
    this->m_y = ay;
    this->m_width = aaw;
    this->m_height = aah;
}

constexpr inline void Rect::getCoords(float *xp1, float *yp1, float *xp2, float *yp2) const noexcept
{
    *xp1 = m_x;
    *yp1 = m_y;
    *xp2 = m_x + m_width;
    *yp2 = m_y + m_height;
}

constexpr inline void Rect::setCoords(float xp1, float yp1, float xp2, float yp2) noexcept
{
    m_x = xp1;
    m_y = yp1;
    m_width = xp2 - xp1;
    m_height = yp2 - yp1;
}

constexpr inline void Rect::adjust(float xp1, float yp1, float xp2, float yp2) noexcept
{
    m_x += xp1;
    m_y += yp1;
    m_width += xp2 - xp1;
    m_height += yp2 - yp1;
}

constexpr inline Rect Rect::adjusted(float xp1, float yp1, float xp2, float yp2) const noexcept
{
    return Rect(m_x + xp1, m_y + yp1, m_width + xp2 - xp1, m_height + yp2 - yp1);
}

constexpr inline void Rect::setWidth(float aw) noexcept
{
    this->m_width = aw;
}

constexpr inline void Rect::setHeight(float ah) noexcept
{
    this->m_height = ah;
}

constexpr inline void Rect::setSize(const Size &s) noexcept
{
    m_width = s.width();
    m_height = s.height();
}

constexpr inline bool Rect::contains(float ax, float ay) const noexcept
{
    return contains(Point(ax, ay));
}

constexpr inline Rect& Rect::operator|=(const Rect &r) noexcept
{
    *this = *this | r;
    return *this;
}

constexpr inline Rect& Rect::operator&=(const Rect &r) noexcept
{
    *this = *this & r;
    return *this;
}

constexpr inline Rect Rect::intersected(const Rect &r) const noexcept
{
    return *this & r;
}

constexpr inline Rect Rect::united(const Rect &r) const noexcept
{
    return *this | r;
}

}

inline bool operator==(const tunis::Rect &r1, const tunis::Rect &r2) noexcept
{
    return glm::epsilonEqual(r1.x(), r2.x(), glm::epsilon<float>()) &&
           glm::epsilonEqual(r1.y(), r2.y(), glm::epsilon<float>()) &&
           glm::epsilonEqual(r1.width(), r2.width(), glm::epsilon<float>()) &&
           glm::epsilonEqual(r1.height(), r2.height(), glm::epsilon<float>());
}

inline bool operator!=(const tunis::Rect &r1, const tunis::Rect &r2) noexcept
{
    return glm::epsilonNotEqual(r1.x(), r2.x(), glm::epsilon<float>()) ||
           glm::epsilonNotEqual(r1.y(), r2.y(), glm::epsilon<float>()) ||
           glm::epsilonNotEqual(r1.width(), r2.width(), glm::epsilon<float>()) ||
           glm::epsilonNotEqual(r1.height(), r2.height(), glm::epsilon<float>());
}

