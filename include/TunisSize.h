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

namespace tunis
{

enum AspectRatioMode {
    IgnoreAspectRatio,
    KeepAspectRatio,
    KeepAspectRatioByExpanding
};

class Size
{
public:
    constexpr Size() noexcept;
    constexpr Size(const Size &sz) noexcept;
    constexpr Size(float w, float h) noexcept;
    inline bool isNull() const noexcept;
    constexpr inline bool isEmpty() const noexcept;
    constexpr inline bool isValid() const noexcept;
    constexpr inline float &width() noexcept;
    constexpr inline float &height() noexcept;
    constexpr inline float width() const noexcept;
    constexpr inline float height() const noexcept;
    constexpr inline void setWidth(float w) noexcept;
    constexpr inline void setHeight(float h) noexcept;
    constexpr void transpose() noexcept;
    constexpr inline Size transposed() const noexcept;
    constexpr void scale(float w, float h, AspectRatioMode mode = IgnoreAspectRatio) noexcept;
    constexpr void scale(const Size &s, AspectRatioMode mode = IgnoreAspectRatio) noexcept;
    constexpr Size scaled(float w, float h, AspectRatioMode mode = IgnoreAspectRatio) const noexcept;
    constexpr Size scaled(const Size &s, AspectRatioMode mode = IgnoreAspectRatio) const noexcept;
    constexpr inline Size expandedTo(const Size &) const noexcept;
    constexpr inline Size boundedTo(const Size &) const noexcept;
    constexpr inline Size &operator+=(const Size &) noexcept;
    constexpr inline Size &operator-=(const Size &) noexcept;
    constexpr inline Size &operator*=(float c) noexcept;
    constexpr inline const Size operator+(const Size &) noexcept;
    constexpr inline const Size operator-(const Size &) noexcept;
    constexpr inline const Size operator*(float) noexcept;
    constexpr inline const Size operator*(const Size &) noexcept;
    constexpr inline Size &operator/=(float c);
    constexpr inline const Size operator/(float);
private:
    float m_width;
    float m_height;
};


}

#include <TunisSize.inl>

#endif // TUNISSIZE_H
