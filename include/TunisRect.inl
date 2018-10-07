/**
 * MIT License
 *
 * Copyright (c) 2018 Matt Chiasson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/
#include <TunisRect.h>

#include <glm/gtc/epsilon.hpp>

namespace tunis
{

constexpr Rect::Rect() noexcept :
    m_x(.0f),
    m_y(.0f),
    m_width(.0f),
    m_height(.0f)
{
}

constexpr Rect::Rect(float x, float y, float width, float height) noexcept :
    m_x(x),
    m_y(y),
    m_width(width),
    m_height(height)
{
}

constexpr Rect::Rect(const Point &topLeft, const Size &size) noexcept :
    m_x(topLeft.x),
    m_y(topLeft.y),
    m_width(size.width()),
    m_height(size.height())
{
}

constexpr Rect::Rect(const Point &topLeft, const Point &bottomRight) noexcept :
    m_x(topLeft.x),
    m_y(topLeft.y),
    m_width(bottomRight.x - topLeft.x),
    m_height(bottomRight.y - topLeft.y)
{
}

constexpr Rect::Rect(const Rect &other) noexcept :
    m_x(other.m_x),
    m_y(other.m_y),
    m_width(other.m_width),
    m_height(other.m_height)
{
}

inline bool Rect::isNull() const noexcept
{
    return glm::epsilonEqual(m_width, 0.0f, glm::epsilon<float>()) &&
            glm::epsilonEqual(m_height, 0.0f, glm::epsilon<float>());
}
constexpr bool Rect::isEmpty() const noexcept
{
    return m_width <= .0f || m_height <= .0f;
}

constexpr bool Rect::isValid() const noexcept
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

constexpr float &Rect::x() noexcept
{
    return m_x;
}

constexpr float Rect::x() const noexcept
{
    return m_x;
}

constexpr float &Rect::y() noexcept
{
    return m_y;
}

constexpr float Rect::y() const noexcept
{
    return m_y;
}

constexpr void Rect::setLeft(float left) noexcept
{
    float diff = left - m_x;
    m_x += diff;
    m_width -= diff;
}

constexpr void Rect::setRight(float right) noexcept
{
    m_width = right - m_x;
}

constexpr void Rect::setTop(float top) noexcept
{
    float diff = top - m_y;
    m_y += diff;
    m_height -= diff;
}

constexpr void Rect::setBottom(float bottom) noexcept
{
    m_height = bottom - m_y;
}

constexpr void Rect::setTopLeft(const Point &topLeft) noexcept
{
    setLeft(topLeft.x);
    setTop(topLeft.y);
}

constexpr void Rect::setTopRight(const Point &topRight) noexcept
{
    setRight(topRight.x);
    setTop(topRight.y);
}

constexpr void Rect::setBottomLeft(const Point &bottomLeft) noexcept
{
    setLeft(bottomLeft.x);
    setBottom(bottomLeft.y);
}

constexpr void Rect::setBottomRight(const Point &bottomRight) noexcept
{
    setRight(bottomRight.x);
    setBottom(bottomRight.y);
}

constexpr void Rect::setX(float pos) noexcept
{
    setLeft(pos);
}

constexpr void Rect::setY(float pos) noexcept
{
    setTop(pos);
}

constexpr Point Rect::topLeft() const noexcept
{
    return Point(m_x, m_y);
}

constexpr Point Rect::bottomRight() const noexcept
{
    return Point(m_x+m_width, m_y+m_height);
}

constexpr Point Rect::topRight() const noexcept
{
    return Point(m_x+m_width, m_y);
}

constexpr Point Rect::bottomLeft() const noexcept
{
    return Point(m_x, m_y+m_height);
}

constexpr Point Rect::center() const noexcept
{
    return Point(m_x + m_width/2, m_y + m_height/2);
}

constexpr void Rect::moveLeft(float pos) noexcept
{
    m_x = pos;
}

constexpr void Rect::moveTop(float pos) noexcept
{
    m_y = pos;
}

constexpr void Rect::moveRight(float pos) noexcept
{
    m_x = pos - m_width;
}

constexpr void Rect::moveBottom(float pos) noexcept
{
    m_y = pos - m_height;
}

constexpr void Rect::moveTopLeft(const Point &p) noexcept
{
    moveLeft(p.x);
    moveTop(p.y);
}

constexpr void Rect::moveTopRight(const Point &p) noexcept
{
    moveRight(p.x);
    moveTop(p.y);
}

constexpr void Rect::moveBottomLeft(const Point &p) noexcept
{
    moveLeft(p.x);
    moveBottom(p.y);
}

constexpr void Rect::moveBottomRight(const Point &p) noexcept
{
    moveRight(p.x);
    moveBottom(p.y);
}

constexpr void Rect::moveCenter(const Point &p) noexcept
{
    m_x = p.x - m_width/2;
    m_y = p.y - m_height/2;
}
constexpr float Rect::width() const noexcept
{
    return m_width;
}

constexpr float Rect::height() const noexcept
{
    return m_height;
}

constexpr Size Rect::size() const noexcept
{
    return Size(m_width, m_height);
}

constexpr void Rect::translate(float dx, float dy) noexcept
{
    m_x += dx;
    m_y += dy;
}

constexpr void Rect::translate(const Point &p) noexcept
{
    m_x += p.x;
    m_y += p.y;
}

constexpr void Rect::moveTo(float ax, float ay) noexcept
{
    m_x = ax;
    m_y = ay;
}

constexpr void Rect::moveTo(const Point &p) noexcept
{
    m_x = p.x;
    m_y = p.y;
}

constexpr Rect Rect::translated(float dx, float dy) const noexcept
{
    return Rect(m_x + dx, m_y + dy, m_width, m_height);
}

constexpr Rect Rect::translated(const Point &p) const noexcept
{
    return Rect(m_x + p.x, m_y + p.y, m_width, m_height);
}

constexpr Rect Rect::transposed() const noexcept
{
    return Rect(topLeft(), size().transposed());
}

constexpr void Rect::getRect(float *ax, float *ay, float *aaw, float *aah) const noexcept
{
    *ax = this->m_x;
    *ay = this->m_y;
    *aaw = this->m_width;
    *aah = this->m_height;
}

constexpr void Rect::setRect(float ax, float ay, float aaw, float aah) noexcept
{
    this->m_x = ax;
    this->m_y = ay;
    this->m_width = aaw;
    this->m_height = aah;
}

constexpr void Rect::getCoords(float *xp1, float *yp1, float *xp2, float *yp2) const noexcept
{
    *xp1 = m_x;
    *yp1 = m_y;
    *xp2 = m_x + m_width;
    *yp2 = m_y + m_height;
}

constexpr void Rect::setCoords(float xp1, float yp1, float xp2, float yp2) noexcept
{
    m_x = xp1;
    m_y = yp1;
    m_width = xp2 - xp1;
    m_height = yp2 - yp1;
}

constexpr void Rect::adjust(float xp1, float yp1, float xp2, float yp2) noexcept
{
    m_x += xp1;
    m_y += yp1;
    m_width += xp2 - xp1;
    m_height += yp2 - yp1;
}

constexpr Rect Rect::adjusted(float xp1, float yp1, float xp2, float yp2) const noexcept
{
    return Rect(m_x + xp1, m_y + yp1, m_width + xp2 - xp1, m_height + yp2 - yp1);
}

constexpr void Rect::setWidth(float aw) noexcept
{
    this->m_width = aw;
}

constexpr void Rect::setHeight(float ah) noexcept
{
    this->m_height = ah;
}

constexpr void Rect::setSize(const Size &s) noexcept
{
    m_width = s.width();
    m_height = s.height();
}

constexpr bool Rect::contains(float ax, float ay) const noexcept
{
    return contains(Point(ax, ay));
}

constexpr Rect& Rect::operator|=(const Rect &r) noexcept
{
    *this = *this | r;
    return *this;
}

constexpr Rect& Rect::operator&=(const Rect &r) noexcept
{
    *this = *this & r;
    return *this;
}

constexpr Rect Rect::intersected(const Rect &r) const noexcept
{
    return *this & r;
}

constexpr Rect Rect::united(const Rect &r) const noexcept
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

