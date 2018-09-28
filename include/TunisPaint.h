#ifndef TUNISPAINT_H
#define TUNISPAINT_H

#include <TunisColor.h>
#include <TunisGradient.h>
#include <TunisSOA.h>
#include <TunisTypes.h>

#include <algorithm>
#include <array>

namespace tunis
{

    class Paint : public RefCountedSOA<
            SVGMatrix,
            Position,
            float,
            float,
            Color,
            Color,
            size_t>
    {
    public:

        inline SVGMatrix &xform() { return get<0>(); }
        inline Position &extend() { return get<1>(); }
        inline float & radius() { return get<2>(); }
        inline float & feather() { return get<3>(); }
        inline Color & innerColor() { return get<4>(); }
        inline Color & outerColor() { return get<5>(); }
        inline size_t & image() { return get<6>(); }

        inline const SVGMatrix &xform() const { return get<0>(); }
        inline const Position &extend() const { return get<1>(); }
        inline const float & radius() const { return get<2>(); }
        inline const float & feather() const { return get<3>(); }
        inline const Color & innerColor() const{ return get<4>(); }
        inline const Color & outerColor() const { return get<5>(); }
        inline const size_t & image() const { return get<6>(); }

        Paint();
        Paint(const Color &color);
        Paint(const char* colorName);
        Paint(const Gradient &gradient);

    };

}

#include "TunisPaint.inl"

#endif // TUNISPAINT_H
