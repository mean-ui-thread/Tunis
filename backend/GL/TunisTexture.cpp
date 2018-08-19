#include "TunisTexture.h"
#include "TunisBackend.h"

#include "TunisGL.h"

#include "soa.h"

using namespace tunis;

enum {
    _handle = 0,
    _filtering,
    _refCount
};

static SoA<
    GLuint,             // _handle
    Texture::Filtering, // _filtering
    int                 // _refCount
> _soa;

static std::vector<size_t> _available;

Texture::Texture() : id(0)
{
    // zero is reserved for the 'null texture'
    if (_soa.size() == 0)
    {
        _soa.resize(1);
    }

    _soa.get<_handle>(id) = 0;
    _soa.get<_filtering>(id) = FilterNone;
    _soa.get<_refCount>(id) = 0xFFFF; // can never be sent back to the _available pool.
}

Texture::Texture(int width, int height, Filtering filtering)
{
    GLuint handle;

    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

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
    case FilterNone:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    case FilterBilinear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case FilterBilinearMipmap:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;
    case FilterTrilinear:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
        break;
    }

    if (_available.size() > 0)
    {
        id = _available.back();
        _available.pop_back();
    }
    else
    {
        id = _soa.size();
        _soa.resize(_soa.size()+1);
    }

    _soa.get<_handle>(id) = handle;
    _soa.get<_filtering>(id) = filtering;
    _soa.get<_refCount>(id) = 1;
}

Texture::Texture(const Texture &other) : id(other.id)
{
    ++_soa.get<_refCount>(id);
}

Texture::~Texture()
{
    if (--_soa.get<_refCount>(id) == 0)
    {
        glDeleteTextures(1, &_soa.get<_handle>(id));
        _available.push_back(id);
    }
}

Texture &Texture::operator=(const Texture &other)
{
    if (this != &other)
    {
        if (--_soa.get<_refCount>(id) == 0)
        {
            glDeleteTextures(1, &_soa.get<_handle>(id));
            _available.push_back(id);
        }

        id = other.id;
        ++_soa.get<_refCount>(id);
    }

    return *this;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, _soa.get<_handle>(id));
}
