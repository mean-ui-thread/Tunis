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
#include <TunisTexture.h>
#include <TunisGL.h>

#include <TunisGraphicStates.h>

#include <soa.h>

namespace tunis
{
    namespace detail
    {
        Texture::Texture(int width, int height, Filtering filtering) :
            width(width),
            height(height),
            filtering(filtering)
        {
            glGenTextures(1, &handle);
            glBindTexture(GL_TEXTURE_2D, handle);
            gfxStates.textureId = handle;

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

            // we're now going to draw a 64x64 px white rectangle starting at the 0, 0
            // corner that we're going to be using to render solid color without having
            // to switch shaders, allowing the batch to continue without interruptions.

            std::vector<uint8_t> whiteSubTexture(64*64*4, 0xFF);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 64, GL_RGBA, GL_UNSIGNED_BYTE, &whiteSubTexture.front());

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            switch(filtering)
            {
                case Filtering::none:
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    break;
                case Filtering::bilinear:
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    break;
                case Filtering::bilinearMipmap:
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    break;
                case Filtering::trilinear:
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glGenerateMipmap(GL_TEXTURE_2D);
                    break;
            }
        }

        Texture::~Texture()
        {
            if (gfxStates.textureId == handle)
            {
                gfxStates.textureId = 0;
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            glDeleteTextures(1, &handle);
            handle = 0;
        }

        void Texture::bind()
        {
            if (gfxStates.textureId != handle)
            {
                glBindTexture(GL_TEXTURE_2D, handle);
                gfxStates.textureId = handle;
            }
        }

    }

}
