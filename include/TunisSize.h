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
#ifndef TUNISSIZE_H
#define TUNISSIZE_H

#include <cinttypes>

namespace tunis
{

    enum AspectRatioMode {
        IgnoreAspectRatio,
        KeepAspectRatio,
        KeepAspectRatioByExpanding
    };

    template<typename T>
    class Size
    {
    public:
        constexpr Size() noexcept;
        constexpr Size(const Size &sz) noexcept;
        constexpr Size(T w, T h) noexcept;
        inline bool isNull() const noexcept;
        constexpr inline bool isEmpty() const noexcept;
        constexpr inline bool isValid() const noexcept;
        constexpr inline T &width() noexcept;
        constexpr inline T &height() noexcept;
        constexpr inline T width() const noexcept;
        constexpr inline T height() const noexcept;
        constexpr inline void setWidth(T w) noexcept;
        constexpr inline void setHeight(T h) noexcept;
        constexpr inline Size &operator+=(const Size &) noexcept;
        constexpr inline Size &operator-=(const Size &) noexcept;
        constexpr inline Size &operator*=(T c) noexcept;
        constexpr inline const Size operator+(const Size &) noexcept;
        constexpr inline const Size operator-(const Size &) noexcept;
        constexpr inline const Size operator*(T) noexcept;
        constexpr inline const Size operator*(const Size &) noexcept;
        constexpr inline Size &operator/=(T c);
        constexpr inline const Size operator/(T);
    private:
        T m_width;
        T m_height;
    };


}

#include <TunisSize.inl>

#endif // TUNISSIZE_H
