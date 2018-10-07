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
#ifndef TUNISRECT_H
#define TUNISRECT_H

#include <TunisTypes.h>
#include <TunisSize.h>

namespace tunis
{

class Rect
{
public:
    constexpr Rect() noexcept;
    constexpr Rect(const Point &topleft, const Size &size) noexcept;
    constexpr Rect(const Point &topleft, const Point &bottomRight) noexcept;
    constexpr Rect(float x, float y, float width, float height) noexcept;
    constexpr Rect(const Rect &rect) noexcept;
    bool isNull() const noexcept;
    constexpr bool isEmpty() const noexcept;
    constexpr bool isValid() const noexcept;
    constexpr Rect normalized() const noexcept;
    constexpr float left() const noexcept;
    constexpr float top() const noexcept;
    constexpr float right() const noexcept;
    constexpr float bottom() const noexcept;
    constexpr float &x() noexcept;
    constexpr float x() const noexcept;
    constexpr float &y() noexcept;
    constexpr float y() const noexcept;
    constexpr void setLeft(float pos) noexcept;
    constexpr void setTop(float pos) noexcept;
    constexpr void setRight(float pos) noexcept;
    constexpr void setBottom(float pos) noexcept;
    constexpr void setX(float pos) noexcept;
    constexpr void setY(float pos) noexcept;
    constexpr Point topLeft() const noexcept;
    constexpr Point bottomRight() const noexcept;
    constexpr Point topRight() const noexcept;
    constexpr Point bottomLeft() const noexcept;
    constexpr Point center() const noexcept;
    constexpr void setTopLeft(const Point &p) noexcept;
    constexpr void setBottomRight(const Point &p) noexcept;
    constexpr void setTopRight(const Point &p) noexcept;
    constexpr void setBottomLeft(const Point &p) noexcept;
    constexpr void moveLeft(float pos) noexcept;
    constexpr void moveTop(float pos) noexcept;
    constexpr void moveRight(float pos) noexcept;
    constexpr void moveBottom(float pos) noexcept;
    constexpr void moveTopLeft(const Point &p) noexcept;
    constexpr void moveBottomRight(const Point &p) noexcept;
    constexpr void moveTopRight(const Point &p) noexcept;
    constexpr void moveBottomLeft(const Point &p) noexcept;
    constexpr void moveCenter(const Point &p) noexcept;
    constexpr void translate(float dx, float dy) noexcept;
    constexpr void translate(const Point &p) noexcept;
    constexpr Rect translated(float dx, float dy) const noexcept;
    constexpr Rect translated(const Point &p) const noexcept;
    constexpr Rect transposed() const noexcept;
    constexpr void moveTo(float x, float y) noexcept;
    constexpr void moveTo(const Point &p) noexcept;
    constexpr void setRect(float x, float y, float m_width, float m_height) noexcept;
    constexpr void getRect(float *x, float *y, float *m_width, float *m_height) const noexcept;
    constexpr void setCoords(float x1, float y1, float x2, float y2) noexcept;
    constexpr void getCoords(float *x1, float *y1, float *x2, float *y2) const noexcept;
    constexpr void adjust(float x1, float y1, float x2, float y2) noexcept;
    constexpr Rect adjusted(float x1, float y1, float x2, float y2) const noexcept;
    constexpr Size size() const noexcept;
    constexpr float width() const noexcept;
    constexpr float height() const noexcept;
    constexpr void setWidth(float m_width) noexcept;
    constexpr void setHeight(float m_height) noexcept;
    constexpr void setSize(const Size &s) noexcept;
    constexpr Rect operator|(const Rect &r) const noexcept;
    constexpr Rect operator&(const Rect &r) const noexcept;
    constexpr Rect& operator|=(const Rect &r) noexcept;
    constexpr Rect& operator&=(const Rect &r) noexcept;
    constexpr bool contains(const Rect &r) const noexcept;
    constexpr bool contains(const Point &p) const noexcept;
    constexpr bool contains(float x, float y) const noexcept;
    constexpr Rect united(const Rect &other) const noexcept;
    constexpr Rect intersected(const Rect &other) const noexcept;
    constexpr bool intersects(const Rect &r) const noexcept;
private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
};

}


#include <TunisRect.inl>


#endif // TUNISRECT_H
