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

#include <TunisSize.h>

#include <glm/common.hpp>

namespace tunis
{

    template<typename T>
    constexpr inline Size<T>::Size() noexcept
        : m_width(-1), m_height(-1)
    {

    }

    template<typename T>
    constexpr inline Size<T>::Size(const Size &other) noexcept
        : m_width(other.m_width), m_height(other.m_height)
    {

    }

    template<typename T>
    constexpr inline Size<T>::Size(T w, T h) noexcept
        : m_width(w), m_height(h)
    {

    }

    template<typename T>
    inline bool Size<T>::isNull() const noexcept
    {
        return m_width == 0 &&
                m_height == 0;
    }

    template<typename T>
    constexpr inline bool Size<T>::isEmpty() const noexcept
    {
        return m_width <= 0 || m_height <= 0;
    }

    template<typename T>
    constexpr inline bool Size<T>::isValid() const noexcept
    {
        return m_width >= 0 && m_height >= 0;
    }

    template<typename T>
    constexpr inline T &Size<T>::width() noexcept
    {
        return m_width;
    }

    template<typename T>
    constexpr inline T &Size<T>::height() noexcept
    {
        return m_height;
    }

    template<typename T>
    constexpr inline T Size<T>::width() const noexcept
    {
        return m_width;
    }

    template<typename T>
    constexpr inline T Size<T>::height() const noexcept
    {
        return m_height;
    }

    template<typename T>
    constexpr inline void Size<T>::setWidth(T w) noexcept
    {
        m_width = w;
    }

    template<typename T>
    constexpr inline void Size<T>::setHeight(T h) noexcept
    {
        m_height = h;
    }

    template<typename T>
    constexpr inline Size<T> &Size<T>::operator+=(const Size &other) noexcept
    {
        m_width += other.m_width; m_height += other.m_height; return *this;
    }

    template<typename T>
    constexpr inline Size<T> &Size<T>::operator-=(const Size &other) noexcept
    {
        m_width -= other.m_width; m_height -= other.m_height; return *this;
    }

    template<typename T>
    constexpr inline Size<T> &Size<T>::operator*=(T c) noexcept
    {
        m_width *= c; m_height *= c; return *this;
    }

    template<typename T>
    constexpr inline const Size<T> Size<T>::operator+(const Size &other) noexcept
    {
        return Size(m_width + other.m_width, m_height + other.m_height);
    }

    template<typename T>
    constexpr inline const Size<T> Size<T>::operator-(const Size &other) noexcept
    {
        return Size(m_width - other.m_width, m_height - other.m_height);
    }

    template<typename T>
    constexpr inline const Size<T> Size<T>::operator*(T v) noexcept
    {
        return Size(m_width * v, m_height * v);
    }

    template<typename T>
    constexpr inline const Size<T> Size<T>::operator*(const Size &other) noexcept
    {
        return Size(m_width * other.m_width, m_height * other.m_height);
    }

    template<typename T>
    constexpr inline Size<T> &Size<T>::operator/=(T v)
    {
        assert(v != 0);
        m_width = m_width/v; m_height = m_height/v;
        return *this;
    }

    template<typename T>
    constexpr inline const Size<T> Size<T>::operator/(T v)
    {
        assert(v != 0);
        return Size(m_width / v, m_height / v);
    }
}

template<typename T>
inline bool operator==(const tunis::Size<T> &lhs, const tunis::Size<T> &rhs) noexcept
{
    return lhs.width() == rhs.width() &&
            lhs.height() == rhs.height();
}

template<typename T>
inline bool operator!=(const tunis::Size<T> &lhs, const tunis::Size<T> &rhs) noexcept
{
    return lhs.width() == rhs.width() ||
            lhs.height() == rhs.height();
}

