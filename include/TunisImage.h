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
#ifndef TUNISIMAGE_H
#define TUNISIMAGE_H

#include <TunisSOA.h>
#include <TunisRect.h>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <string>

namespace tunis
{

    class Context;

    namespace detail
    {
        class ContextPriv;
        class Texture;
    }

    class Image : public RefCountedSOA<std::string, std::vector<uint8_t>, Rect<int32_t>, Rect<int32_t>, detail::Texture*>
    {
        inline std::string &source() { return get<0>(); }
        inline std::vector<uint8_t> &data() { return get<1>(); }
        inline Rect<int32_t> &bounds() { return get<2>(); }
        inline Rect<int32_t> &paddedBounds() { return get<3>(); }
        inline detail::Texture* &parent() { return get<4>(); }

        inline const std::string &source() const { return get<0>(); }
        inline const std::vector<uint8_t> &data() const { return get<1>(); }
        inline const Rect<int32_t> &bounds() const { return get<2>(); }
        inline const Rect<int32_t> &paddedBounds() const { return get<3>(); }
        inline const detail::Texture* parent() const { return get<4>(); }

        friend detail::ContextPriv;
        friend detail::Texture;

        class Source
        {
        public:
            Image *image;
            Source(Image *image);
            Source(const Source &other);
            Source &operator=(const Source &other);
            std::string &operator=(std::string value);
            operator std::string &();
            operator const std::string &() const;
        };

        void sourceChanged(detail::ContextPriv *ctx);
        void dataChanged(detail::ContextPriv *ctx);

    public:

        Image();
        Image(std::string url);

        Source src;
    };
}

#include "TunisImage.inl"

#endif // TUNISIMAGE_H
