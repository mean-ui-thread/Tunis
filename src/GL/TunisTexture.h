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
#ifndef TUNISTEXTURE_H
#define TUNISTEXTURE_H

#include <cinttypes>
#include <cstddef>

#include <TunisGL.h>
#include <TunisImage.h>

namespace tunis
{
    namespace detail
    {
        enum class Filtering
        {
            none, bilinear, bilinearMipmap, trilinear
        };

        class Texture
        {
        public:
            Texture(int width, int height, Filtering filtering = Filtering::bilinear);
            ~Texture();

            bool tryAddImage(Image &img);

            void bind();
            void updateMipmap();
            operator GLuint() const;

        private:

            GLuint handle;
            int32_t width, height;
            Filtering filtering;
            bool mipmapDirty;
            std::vector<Image> images;
        };
    }


}

#include "TunisTexture.inl"

#endif // TUNISTEXTURE_H
