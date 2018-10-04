#ifndef TUNISGRADIENT_H
#define TUNISGRADIENT_H

#include <TunisColor.h>
#include <TunisSOA.h>

namespace tunis
{
    namespace detail
    {
        struct ColorStopArray : public SoA<float, Color>
        {
            inline float &offset(size_t idx) { return get<0>(idx); }
            inline Color &color(size_t idx) { return get<1>(idx); }

            inline const float &offset(size_t idx) const { return get<0>(idx); }
            inline const Color &color(size_t idx) const { return get<1>(idx); }
        };
    }

    /*!
     * \brief The Gradient class represents an object describing a gradient. It
     * is returned by the methods Context::createLinearGradient() or
     * Context::createRadialGradient().
     *
     * \note It can be used as a fillStyle or strokeStyle.
     */
    class Gradient : public RefCountedSOA<glm::vec2, glm::vec2, glm::vec2, detail::ColorStopArray>
    {
        inline glm::vec2 &start() { return get<0>(); }
        inline glm::vec2 &end() { return get<1>(); }
        inline glm::vec2 &radius() { return get<2>(); }
        inline detail::ColorStopArray &colorStops() { return get<3>(); }

        inline const glm::vec2 &start() const { return get<0>(); }
        inline const glm::vec2 &end() const { return get<1>(); }
        inline const glm::vec2 &radius() const { return get<2>(); }
        inline const detail::ColorStopArray &colorStops() const { return get<3>(); }

        friend class Context;
        friend class Paint;

        Gradient(float x0, float y0, float x1, float y1);
        Gradient(float x0, float y0, float r0,
                 float x1, float y1, float r1);

    public:

        /*!
         * \brief addColorStop adds a new stop, defined by an offset and a
         * color, to the gradient.
         *
         * \param offset A number between 0.0f and 1.0f.
         * \param color A color.
         */
        void addColorStop(float offset, Color color);

    };
}

#include "TunisGradient.inl"

#endif // TUNISGRADIENT_H
