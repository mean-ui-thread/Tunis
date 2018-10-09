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
#ifndef TUNISPAINT_H
#define TUNISPAINT_H

#include <TunisColor.h>
#include <TunisGradient.h>
#include <TunisSOA.h>
#include <TunisTypes.h>
#include <TunisPattern.h>

#include <algorithm>
#include <array>

namespace tunis
{

    namespace detail
    {
        enum class PaintType : uint8_t
        {
            texture, gradientLinear, gradientRadial
        };

        class ContextPriv;
    }

    class Paint : public RefCountedSOA<
            detail::PaintType,
            glm::vec2, glm::vec2, glm::vec2, detail::ColorStopArray,
            Image, RepeatType>
    {
        inline detail::PaintType &type() { return get<0>(); }
        inline glm::vec2 &start() { return get<1>(); }
        inline glm::vec2 &end() { return get<2>(); }
        inline glm::vec2 &radius() { return get<3>(); }
        inline detail::ColorStopArray &colorStops() { return get<4>(); }
        inline Image & image(){ return get<5>(); }
        inline RepeatType& repetition() { return get<6>(); }

        inline const detail::PaintType &type() const { return get<0>(); }
        inline const glm::vec2 &start() const { return get<1>(); }
        inline const glm::vec2 &end() const { return get<2>(); }
        inline const glm::vec2 &radius() const { return get<3>(); }
        inline const detail::ColorStopArray &colorStops() const { return get<4>(); }
        inline const Image &image() const { return get<5>(); }
        inline const RepeatType& repetition() const{ return get<6>(); }

        friend detail::ContextPriv;

    public:
        Paint();
        Paint(const Color &color);
        Paint(const char* colorName);
        Paint(const Gradient &gradient);
        Paint(const Pattern &pattern);

    };

}

#include "TunisPaint.inl"

#endif // TUNISPAINT_H
