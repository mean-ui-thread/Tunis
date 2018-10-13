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
        Rect();
        Rect(const Point<T> &topleft, const Size<T> &size);
        Rect(const Point<T> &topleft, const Point<T> &bottomRight);
        Rect(T x, T y, T width, T height);
        Rect(const Rect &rect);
        bool isNull() const;
        bool isEmpty() const;
        bool isValid() const;
        Rect normalized() const;
        T left() const;
        T top() const;
        T right() const;
        T bottom() const;
        T &x();
        T x() const;
        T &y();
        T y() const;
        void setLeft(T pos);
        void setTop(T pos);
        void setRight(T pos);
        void setBottom(T pos);
        void setX(T pos);
        void setY(T pos);
        Point<T> topLeft() const;
        Point<T> bottomRight() const;
        Point<T> topRight() const;
        Point<T> bottomLeft() const;
        Point<T> center() const;
        T width() const;
        T height() const;
        void setWidth(T m_width);
        void setHeight(T m_height);
        void setSize(const Size<T> &s);
        Rect operator|(const Rect &r) const;
        Rect operator&(const Rect &r) const;
        Rect& operator|=(const Rect &r);
        Rect& operator&=(const Rect &r);
        bool contains(const Rect &r) const;
        bool contains(const Point<T> &p) const;
        bool contains(T x, T y) const;
        Rect united(const Rect &other) const;
        Rect intersected(const Rect &other) const;
        bool intersects(const Rect &r) const;
    private:
        T m_x;
        T m_y;
        T m_width;
        T m_height;
    };

}


#include <TunisRect.inl>


#endif // TUNISRECT_H
