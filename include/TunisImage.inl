#include "TunisImage.h"

#include "TunisTaskQueue.h"

#include <glm/vec4.hpp>

namespace tunis
{
    namespace detail
    {
        extern Image blankImage;
    }

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
        sourceWidth() = 0;
        sourceHeight() = 0;
        bounds() = glm::u16vec4(0);
        parent() = nullptr;
    }

    inline Image::Image(std::string url) :
        src(this)
    {
        source() = std::move(url);
        data().resize(0);
        sourceWidth() = 0;
        sourceHeight() = 0;
        bounds() = glm::u16vec4(0);
        parent() = nullptr;
        detail::enqueueTask(&Image::sourceChanged, this);
    }

    inline Image::Image(const uint8_t *raw, int width, int height) :
        src(this)
    {
        size_t dataSize = width * height * 4;
        source().resize(0);
        data().resize(dataSize);
        memcpy(&data()[0], &raw[0], dataSize);
        sourceWidth() = width;
        sourceHeight() = height;
        bounds() = glm::u16vec4(0);
        parent() = nullptr;
        detail::enqueueTask(&Image::dataChanged, this);
    }

}
