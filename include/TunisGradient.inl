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
#include <TunisGradient.h>

#include <glm/vec2.hpp>

namespace tunis
{
    inline Gradient::Gradient(float x0, float y0, float x1, float y1)
    {
        type() = detail::GradientType::gradientLinear;
        start().x = x0;
        start().y = y0;
        end().x = x1;
        end().y = y1;
        colorStops().resize(0);
    }

    inline Gradient::Gradient(float x0, float y0, float r0,
                              float x1, float y1, float r1)
    {
        type() = detail::GradientType::gradientRadial;
        start().x = x0;
        start().y = y0;
        radius()[0] = r0;
        end().x = x1;
        end().y = y1;
        radius()[1] = r1;
        colorStops().resize(0);
    }

    inline void Gradient::addColorStop(float offset, Color color)
    {
        assert(offset >= 0.0f && offset <= 1.0f);
        colorStops().push(std::move(offset), std::move(color));
    }
}


