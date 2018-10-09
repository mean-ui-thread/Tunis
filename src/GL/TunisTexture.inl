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
#include <TunisImage.h>

#include <soa.h>
#include <glm/vec4.hpp>

namespace tunis
{
    namespace detail
    {
        Texture::Texture(int width, int height, Filtering filtering) :
            width(width),
            height(height),
            filtering(filtering),
            mipmapDirty(false)
        {
            glGenTextures(1, &handle);
            glBindTexture(GL_TEXTURE_2D, handle);
            gfxStates.textureId = handle;

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

            // we're now going to draw a 64x64 px white rectangle starting at the 0, 0
            // corner that we're going to be using to render solid color without having
            // to switch shaders, allowing the batch to continue without interruptions.

            std::vector<uint8_t> whiteSubTexture(gfxStates.texPadding*gfxStates.texPadding*4, 0xFF);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, gfxStates.texPadding, gfxStates.texPadding, GL_RGBA, GL_UNSIGNED_BYTE, whiteSubTexture.data());

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
                    mipmapDirty = true;
                    break;
                case Filtering::trilinear:
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    mipmapDirty = true;
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

        bool Texture::tryAddImage(Image &image)
        {
            if (image.parent())
            {
                fprintf(stderr, "Image %d ('%s') was already added to texture %d.\n",
                        image.getId(),
                        image.source().c_str(),
                        handle);
                // pretend it was successfully added so it doesn't try to be
                // added to the other texture.  One warning is enough.
                return true;
            }

            auto &paddedBounds = image.paddedBounds();
            if(!paddedBounds.isValid())
            {
                fprintf(stderr, "Image %d ('%s') has invalid padded dimension %d,%d.\n",
                        image.getId(),
                        image.source().c_str(),
                        paddedBounds.width(),
                        paddedBounds .height());
                // pretend it was successfully added so it doesn't try to be
                // added to the other texture.  One warning is enough.
                return true;
            }

            auto &bounds = image.bounds();
            if(!bounds.isValid())
            {
                fprintf(stderr, "Image %d ('%s') has invalid dimension %d,%d.\n",
                        image.getId(),
                        image.source().c_str(),
                        paddedBounds.width(),
                        paddedBounds .height());
                // pretend it was successfully added so it doesn't try to be
                // added to the other texture.  One warning is enough.
                return true;
            }

            int32_t maxWidth = gfxStates.maxTexSize - paddedBounds.width();
            int32_t maxHeight = gfxStates.maxTexSize - paddedBounds.height();
            paddedBounds.setY(0);

            if (images.size() == 0)
            {
                goto SpotFound;
            }

            while(paddedBounds.y() < maxHeight)
            {
                paddedBounds.setX(0);
                while(paddedBounds.x() < maxWidth)
                {
                    bool bOccupied = false;
                    for (size_t i = 0; i < images.size(); ++i)
                    {
                        Image &subImage = images[i];
                        auto &subTexturePaddingBound = subImage.paddedBounds();
                        if (paddedBounds.intersects(subTexturePaddingBound))
                        {
                            bOccupied = true;
                            paddedBounds.setX(subTexturePaddingBound.right());
                        }

                        if (!bOccupied)
                        {
                            goto SpotFound;
                        }
                    }
                }
                paddedBounds.setY(paddedBounds.y()+1);
            }

            // no room to add image to this texture.
            return false;

SpotFound:

            bounds.setX(paddedBounds.x() + gfxStates.texPadding);
            bounds.setY(paddedBounds.y() + gfxStates.texPadding);

            images.push_back(image);

            Texture::bind();
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                            paddedBounds.x(),
                            paddedBounds.y(),
                            paddedBounds.width(),
                            paddedBounds.height(),
                            GL_RGBA, GL_UNSIGNED_BYTE,
                            image.data().data());

            mipmapDirty = filtering == Filtering::bilinearMipmap || filtering == Filtering::trilinear;

            return true;
        }

        void Texture::bind()
        {
            if (gfxStates.textureId != handle)
            {
                glBindTexture(GL_TEXTURE_2D, handle);
                gfxStates.textureId = handle;
            }

        }

        void Texture::updateMipmap()
        {
            if (mipmapDirty)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
                mipmapDirty = false;
            }
        }

    }

}
