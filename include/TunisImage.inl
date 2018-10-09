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
#include "TunisImage.h"

#include "TunisTaskQueue.h"

#include <glm/vec4.hpp>

namespace tunis
{
    inline Image::Source::Source(Image *image) :
        image(image)
    {
    }

    inline Image::Source::Source(const Source &other) :
        image(other.image)
    {
    }

    inline Image::Source &Image::Source::operator=(const Image::Source &other)
    {
        if (&other != this)
        {
            image = other.image;
        }
        return *this;
    }

    inline std::string &Image::Source::operator=(std::string value)
    {
        image->source() = std::move(value);
        detail::enqueueTask(&Image::sourceChanged, image);
        return image->source();
    }

    inline Image::Source::operator std::string &()
    {
        return image->source();
    }

    inline Image::Source::operator const std::string &() const
    {
        return image->source();
    }

    inline Image::Image() :
        src(this)
    {
        source().resize(0);
        data().resize(0);
        bounds() = Rect<int32_t>(0, 0, 1, 1);
        paddedBounds() = Rect<int32_t>();
        parent() = nullptr;
    }

    inline Image::Image(std::string url) :
        src(this)
    {
        source() = std::move(url);
        data().resize(0);
        bounds() = Rect<int32_t>(0, 0, 1, 1);
        paddedBounds() = Rect<int32_t>();
        parent() = nullptr;
        detail::enqueueTask(&Image::sourceChanged, this);
    }

}
