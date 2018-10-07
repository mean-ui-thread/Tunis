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
#include "TunisPaint.h"

#include <cfloat>

#include <glm/geometric.hpp>
#include <glm/gtc/epsilon.hpp>

namespace tunis
{

    inline Paint::Paint()
    {
        type() = detail::PaintType::solid;
        colorStops().resize(1);
        colorStops().color(0) = Black;
        image() = detail::blankImage;
    }

    inline Paint::Paint(const Color &c)
    {
        type() = detail::PaintType::solid;
        colorStops().resize(1);
        colorStops().color(0) = c;
        image() = detail::blankImage;
    }

    inline Paint::Paint(const char* c)
    {
        type() = detail::PaintType::solid;
        colorStops().resize(1);
        colorStops().color(0) = c;
        image() = detail::blankImage;
    }

    inline Paint::Paint(const Gradient &g)
    {
        type() = g.type() == detail::GradientType::gradientLinear ? detail::PaintType::gradientLinear : detail::PaintType::gradientRadial;
        start() = g.start();
        end() = g.end();
        radius() = g.radius();
        colorStops() = g.colorStops();
        image() = detail::blankImage;
    }

    inline Paint::Paint(const Image &img)
    {
        type() = detail::PaintType::image;
        colorStops().resize(1);
        colorStops().color(0) = White;
        image() = img;
    }

}
