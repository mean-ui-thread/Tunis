#ifndef TUNISFONT_H
#define TUNISFONT_H

#include "TunisImage.h"
#include "TunisFontStyle.h"

namespace tunis
{

    struct GlyphArray : public SoA<uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, Image>
    {
        uint32_t &unicode(size_t i) { return get<0>(i); }
        uint32_t &width(size_t i) { return get<1>(i); }
        uint32_t &height(size_t i) { return get<2>(i); }
        uint32_t &xoffset(size_t i) { return get<3>(i); }
        uint32_t &yoffset(size_t i) { return get<4>(i); }
        uint32_t &xadvance(size_t i) { return get<5>(i); }
        uint32_t &yadvance(size_t i) { return get<6>(i); }
        Image &image(size_t i) { return get<7>(i); }

        const uint32_t &unicode(size_t i) const { return get<0>(i); }
        const uint32_t &width(size_t i) const { return get<1>(i); }
        const uint32_t &height(size_t i) const { return get<2>(i); }
        const uint32_t &xoffset(size_t i) const { return get<3>(i); }
        const uint32_t &yoffset(size_t i) const { return get<4>(i); }
        const uint32_t &xadvance(size_t i) const { return get<5>(i); }
        const uint32_t &yadvance(size_t i) const { return get<6>(i); }
        const Image &image(size_t i) const { return get<7>(i); }
    };

    struct FontArray : public SoA<std::string, FontStyle, uint8_t, uint8_t, uint8_t, uint8_t, GlyphArray>
    {
        std::string &family(size_t i) { return get<0>(i); }
        FontStyle &style(size_t i) { return get<1>(i); }
        uint8_t &fontSize(size_t i) { return get<2>(i); }
        uint8_t &range(size_t i) { return get<3>(i); }
        uint8_t &padding(size_t i) { return get<4>(i); }
        uint8_t &lineHeight(size_t i) { return get<5>(i); }
        GlyphArray &glyphs(size_t i) { return get<6>(i); }

        const std::string &family(size_t i) const { return get<0>(i); }
        const FontStyle &style(size_t i) const { return get<1>(i); }
        const uint8_t &fontSize(size_t i) const { return get<2>(i); }
        const uint8_t &range(size_t i) const { return get<3>(i); }
        const uint8_t &padding(size_t i) const { return get<4>(i); }
        const uint8_t &lineHeight(size_t i) const { return get<5>(i); }
        const GlyphArray &glyphs(size_t i) const { return get<6>(i); }
    };

}

#endif // TUNISFONT_H
