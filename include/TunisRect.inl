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

namespace tunis
{
    template<typename T>
    inline Rect<T>::Rect() :
        m_x(0),
        m_y(0),
        m_width(-1),
        m_height(-1)
    {
    }

    template<typename T>
    inline Rect<T>::Rect(T x, T y, T width, T height) :
        m_x(x),
        m_y(y),
        m_width(width),
        m_height(height)
    {
    }

    template<typename T>
    inline Rect<T>::Rect(const Point<T> &topLeft, const Size<T> &size) :
        m_x(topLeft.x),
        m_y(topLeft.y),
        m_width(size.width()),
        m_height(size.height())
    {
    }

    template<typename T>
    inline Rect<T>::Rect(const Point<T> &topLeft, const Point<T> &bottomRight) :
        m_x(topLeft.x),
        m_y(topLeft.y),
        m_width(bottomRight.x - topLeft.x),
        m_height(bottomRight.y - topLeft.y)
    {
    }

    template<typename T>
    inline Rect<T>::Rect(const Rect &other) :
        m_x(other.m_x),
        m_y(other.m_y),
        m_width(other.m_width),
        m_height(other.m_height)
    {
    }

    template<typename T>
    inline bool Rect<T>::isNull() const
    {
        return m_width == 0 && m_height == 0;
    }

    template<typename T>
    inline bool Rect<T>::isEmpty() const
    {
        return m_width <= 0 || m_height <= 0;
    }

    template<typename T>
    inline bool Rect<T>::isValid() const
    {
        return m_width > 0 && m_height > 0;
    }

    template<typename T>
    inline T Rect<T>::left() const
    {
        return m_x;
    }

    template<typename T>
    inline T Rect<T>::top() const
    {
        return m_y;
    }

    template<typename T>
    inline T Rect<T>::right() const
    {
        return m_x + m_width;
    }

    template<typename T>
    inline T Rect<T>::bottom() const
    {
        return m_y + m_height;
    }

    template<typename T>
    inline T &Rect<T>::x()
    {
        return m_x;
    }

    template<typename T>
    inline T Rect<T>::x() const
    {
        return m_x;
    }

    template<typename T>
    inline T &Rect<T>::y()
    {
        return m_y;
    }

    template<typename T>
    inline T Rect<T>::y() const
    {
        return m_y;
    }

    template<typename T>
    inline void Rect<T>::setLeft(T left)
    {
        T diff = left - m_x;
        m_x += diff;
        m_width -= diff;
    }

    template<typename T>
    inline void Rect<T>::setRight(T right)
    {
        m_width = right - m_x;
    }

    template<typename T>
    inline void Rect<T>::setTop(T top)
    {
        T diff = top - m_y;
        m_y += diff;
        m_height -= diff;
    }

    template<typename T>
    inline void Rect<T>::setBottom(T bottom)
    {
        m_height = bottom - m_y;
    }

    template<typename T>
    inline void Rect<T>::setX(T x)
    {
        m_x = x;
    }

    template<typename T>
    inline void Rect<T>::setY(T y)
    {
        m_y = y;
    }

    template<typename T>
    inline Point<T> Rect<T>::topLeft() const
    {
        return Point<T>(m_x, m_y);
    }

    template<typename T>
    inline Point<T> Rect<T>::bottomRight() const
    {
        return Point<T>(m_x+m_width, m_y+m_height);
    }

    template<typename T>
    inline Point<T> Rect<T>::topRight() const
    {
        return Point<T>(m_x+m_width, m_y);
    }

    template<typename T>
    inline Point<T> Rect<T>::bottomLeft() const
    {
        return Point<T>(m_x, m_y+m_height);
    }

    template<typename T>
    inline Point<T> Rect<T>::center() const
    {
        return Point<T>(m_x + m_width/2, m_y + m_height/2);
    }

    template<typename T>
    inline T Rect<T>::width() const
    {
        return m_width;
    }

    template<typename T>
    inline T Rect<T>::height() const
    {
        return m_height;
    }

    template<typename T>
    inline void Rect<T>::setWidth(T w)
    {
        m_width = w;
    }

    template<typename T>
    inline void Rect<T>::setHeight(T h)
    {
        m_height = h;
    }

    template<typename T>
    inline void Rect<T>::setSize(const Size<T> &size)
    {
        m_width = size.width();
        m_height = size.height();
    }

    template<typename T>
    inline bool Rect<T>::contains(T x, T y) const
    {
        return contains(Point<T>(x, y));
    }

    template<typename T>
    inline Rect<T>& Rect<T>::operator|=(const Rect &other)
    {
        *this = *this | other;
        return *this;
    }

    template<typename T>
    inline Rect<T>& Rect<T>::operator&=(const Rect &other)
    {
        *this = *this & other;
        return *this;
    }


    template<typename T>
    inline Rect<T> Rect<T>::united(const Rect &other) const
    {
        return *this | other;
    }

    template<typename T>
    inline Rect<T> Rect<T>::intersected(const Rect &other) const
    {
        return *this & other;
    }

    template<typename T>
    inline bool Rect<T>::intersects(const Rect<T> &other) const
    {
        int left1 = m_x;
        int right1 = m_x;

        if (m_width < 0)
        {
            left1 += m_width;
        }
        else
        {
            right1 += m_width;
        }

        if (left1 == right1)
        {
            return false;
        }

        int left2 = other.m_x;
        int right2 = other.m_x;

        if (other.m_width < 0)
        {
            left2 += other.m_width;
        }
        else
        {
            right2 += other.m_width;
        }

        if (left2 == right2)
        {
            return false;
        }

        if (left1 >= right2 || left2 >= right1)
        {
            return false;
        }

        int top1 = m_y;
        int bottom1 = m_y;

        if (m_height < 0)
        {
            top1 += m_height;
        }
        else
        {
            bottom1 += m_height;
        }

        if (top1 == bottom1)
        {
            return false;
        }

        int top2 = other.m_y;
        int bottom2 = other.m_y;

        if (other.m_height < 0)
        {
            top2 += other.m_height;
        }
        else
        {
            bottom2 += other.m_height;
        }

        if (top2 == bottom2)
        {
            return false;
        }

        if (top1 >= bottom2 || top2 >= bottom1)
        {
            return false;
        }

        return true;
    }
}

template<typename T>
inline bool operator==(const tunis::Rect<T> &lhs, const tunis::Rect<T> &rhs)
{
    return lhs.x() == rhs.x() &&
            lhs.y() == rhs.y() &&
            lhs.width() == rhs.width() &&
            lhs.height() == rhs.height();
}

template<typename T>
inline bool operator!=(const tunis::Rect<T> &lhs, const tunis::Rect<T> &rhs)
{
    return lhs.x() != rhs.x() ||
            lhs.y() != rhs.y() ||
            lhs.width() != rhs.width() ||
            lhs.height() != rhs.height();
}

