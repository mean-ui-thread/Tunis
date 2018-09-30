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
            Color,
            size_t>
    {
    public:

        inline Color & color() { return get<0>(); }
        inline size_t & image(){ return get<1>(); }

        inline const Color & color() const{ return get<0>(); }
        inline const size_t & image() const { return get<1>(); }

        Paint();
        Paint(const Color &color);
        Paint(const char* colorName);
        Paint(const Gradient &gradient);

    };

}

#include "TunisPaint.inl"

#endif // TUNISPAINT_H
