#ifndef TUNISTEXTURE_H
#define TUNISTEXTURE_H

#include <cinttypes>
#include <cstddef>

#include <TunisGL.h>

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
            Texture(int width, int height, Filtering filtering = Filtering::trilinear);
            ~Texture();

            void bind();
            operator GLuint() const;

        private:

            GLuint handle;
            int width, height;
            Filtering filtering;
        };
    }


}

#include "TunisTexture.inl"

#endif // TUNISTEXTURE_H
