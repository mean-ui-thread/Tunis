#include <TunisTexture.h>

#include <nanovg.h>

namespace tunis
{

Texture::Texture() : id(0)
{
}

Texture::Texture(int width, int height, Filtering filtering)
{

}

Texture::Texture(const Texture &other) : id(other.id)
{
}

Texture::Texture(const Texture &&other) : id(other.id)
{
}

Texture::~Texture()
{
}

Texture &Texture::operator=(const Texture &other)
{
    if (this != &other)
    {
        id = other.id;
    }

    return *this;
}

void Texture::bind()
{
}

}
