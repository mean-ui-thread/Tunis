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

    class Paint : public RefCountedSOA<Color, Image>
    {
    public:

        inline Color & color() { return get<0>(); }
        inline Image & image(){ return get<1>(); }

        inline const Color & color() const{ return get<0>(); }
        inline const Image & image() const { return get<1>(); }

        Paint();
        Paint(const Color &color);
        Paint(const char* colorName);
        Paint(const Gradient &gradient);
        Paint(const Image &image);

    };

}

#include "TunisPaint.inl"

#endif // TUNISPAINT_H
