#ifndef TUNISPAINT_H
#define TUNISPAINT_H

#include <TunisColor.h>
#include <TunisGradient.h>
#include <TunisSOA.h>
#include <TunisTypes.h>
#include <TunisImage.h>

#include <algorithm>
#include <array>

namespace tunis
{

    namespace detail
    {
        enum class PaintType : uint8_t
        {
            solid, image, gradient
        };

        class ContextPriv;
    }

    class Paint : public RefCountedSOA<
            detail::PaintType,
            glm::vec2, glm::vec2, glm::vec2, detail::ColorStopArray,
            Image>
    {
        inline detail::PaintType &type() { return get<0>(); }
        inline glm::vec2 &start() { return get<1>(); }
        inline glm::vec2 &end() { return get<2>(); }
        inline glm::vec2 &radius() { return get<3>(); }
        inline detail::ColorStopArray &colorStops() { return get<4>(); }
        inline Image & image(){ return get<5>(); }

        inline const detail::PaintType &type() const { return get<0>(); }
        inline const glm::vec2 &start() const { return get<1>(); }
        inline const glm::vec2 &end() const { return get<2>(); }
        inline const glm::vec2 &radius() const { return get<3>(); }
        inline const detail::ColorStopArray &colorStops() const { return get<4>(); }
        inline const Image &image() const { return get<5>(); }

        friend detail::ContextPriv;

    public:
        Paint();
        Paint(const Color &color);
        Paint(const char* colorName);
        Paint(const Gradient &gradient);
        Paint(const Image &image);

    };

}

#include "TunisPaint.inl"

#endif // TUNISPAINT_H
