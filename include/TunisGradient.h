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
#ifndef TUNISGRADIENT_H
#define TUNISGRADIENT_H

#include <TunisColor.h>
#include <TunisSOA.h>

namespace tunis
{
    namespace detail
    {
        enum class GradientType : uint8_t
        {
            gradientLinear, gradientRadial
        };

        struct ColorStopArray : public SoA<float, Color>
        {
            inline float &offset(size_t idx) { return get<0>(idx); }
            inline Color &color(size_t idx) { return get<1>(idx); }

            inline const float &offset(size_t idx) const { return get<0>(idx); }
            inline const Color &color(size_t idx) const { return get<1>(idx); }
        };
    }

    /*!
     * \brief The Gradient class represents an object describing a gradient. It
     * is returned by the methods Context::createLinearGradient() or
     * Context::createRadialGradient().
     *
     * \note It can be used as a fillStyle or strokeStyle.
     */
    class Gradient : public RefCountedSOA<detail::GradientType, glm::vec2, glm::vec2, glm::vec2, detail::ColorStopArray>
    {
        inline detail::GradientType &type() { return get<0>(); }
        inline glm::vec2 &start() { return get<1>(); }
        inline glm::vec2 &end() { return get<2>(); }
        inline glm::vec2 &radius() { return get<3>(); }
        inline detail::ColorStopArray &colorStops() { return get<4>(); }

        inline const detail::GradientType &type() const { return get<0>(); }
        inline const glm::vec2 &start() const { return get<1>(); }
        inline const glm::vec2 &end() const { return get<2>(); }
        inline const glm::vec2 &radius() const { return get<3>(); }
        inline const detail::ColorStopArray &colorStops() const { return get<4>(); }

        friend class Context;
        friend class Paint;

        Gradient(float x0, float y0, float x1, float y1);
        Gradient(float x0, float y0, float r0,
                 float x1, float y1, float r1);

    public:

        /*!
         * \brief addColorStop adds a new stop, defined by an offset and a
         * color, to the gradient.
         *
         * \param offset A number between 0.0f and 1.0f.
         * \param color A color.
         */
        void addColorStop(float offset, Color color);

    };
}

#include "TunisGradient.inl"

#endif // TUNISGRADIENT_H
