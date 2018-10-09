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
    template <typename T>
    using Point = glm::vec<2, T, glm::highp>;

    template <typename T>
    class Rect
    {
    public:
        constexpr Rect() noexcept;
        constexpr Rect(const Point<T> &topleft, const Size<T> &size) noexcept;
        constexpr Rect(const Point<T> &topleft, const Point<T> &bottomRight) noexcept;
        constexpr Rect(T x, T y, T width, T height) noexcept;
        constexpr Rect(const Rect &rect) noexcept;
        bool isNull() const noexcept;
        constexpr bool isEmpty() const noexcept;
        constexpr bool isValid() const noexcept;
        constexpr Rect normalized() const noexcept;
        constexpr T left() const noexcept;
        constexpr T top() const noexcept;
        constexpr T right() const noexcept;
        constexpr T bottom() const noexcept;
        constexpr T &x() noexcept;
        constexpr T x() const noexcept;
        constexpr T &y() noexcept;
        constexpr T y() const noexcept;
        constexpr void setLeft(T pos) noexcept;
        constexpr void setTop(T pos) noexcept;
        constexpr void setRight(T pos) noexcept;
        constexpr void setBottom(T pos) noexcept;
        constexpr void setX(T pos) noexcept;
        constexpr void setY(T pos) noexcept;
        constexpr Point<T> topLeft() const noexcept;
        constexpr Point<T> bottomRight() const noexcept;
        constexpr Point<T> topRight() const noexcept;
        constexpr Point<T> bottomLeft() const noexcept;
        constexpr Point<T> center() const noexcept;
        constexpr T width() const noexcept;
        constexpr T height() const noexcept;
        constexpr void setWidth(T m_width) noexcept;
        constexpr void setHeight(T m_height) noexcept;
        constexpr void setSize(const Size<T> &s) noexcept;
        constexpr Rect operator|(const Rect &r) const noexcept;
        constexpr Rect operator&(const Rect &r) const noexcept;
        constexpr Rect& operator|=(const Rect &r) noexcept;
        constexpr Rect& operator&=(const Rect &r) noexcept;
        constexpr bool contains(const Rect &r) const noexcept;
        constexpr bool contains(const Point<T> &p) const noexcept;
        constexpr bool contains(T x, T y) const noexcept;
        constexpr Rect united(const Rect &other) const noexcept;
        constexpr Rect intersected(const Rect &other) const noexcept;
        constexpr bool intersects(const Rect &r) const noexcept;
    private:
        T m_x;
        T m_y;
        T m_width;
        T m_height;
    };

}


#include <TunisRect.inl>


#endif // TUNISRECT_H
