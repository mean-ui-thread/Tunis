#include "TunisFontGenerator.h"
#include "TunisFontStyle.h"
#include "TunisGlyphLoader.h"

#include <msdfgen/msdfgen.h>
#include <msdfgen/msdfgen-ext.h>
#include <lodepng.h>

#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/String.h>

#include <array>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstddef>
#include <cfloat>
#include <sstream>


#ifdef _MSC_VER
    #define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#else
    #define PACK( __Declaration__ ) __Declaration__ __attribute__((packed, aligned(1)))
#endif

using namespace tunis;

static const uint16_t s_fontSize = 48;
static const uint8_t s_range = s_fontSize / 8;
static const uint8_t s_padding = 4;

struct RGBA
{
    uint8_t r, g, b, a;
};

static uint32_t unicode_latin1[] = {
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
    0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
    0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
    0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF
};

PACK(struct HeaderChunk {
         char header0 = 'T';
         char header1 = 'F';
         char header2 = 'P';
         uint8_t versionMajor = 0;
         uint8_t versionMinor = 1;
     });

PACK(struct KerningChunk {
         uint8_t chunkID = 0x03;
         uint32_t chunkSize;
         uint32_t unicode_left;
         uint32_t unicode_right;
         int16_t  amount;
     });

PACK(struct GlyphChunk {
         uint8_t chunkID = 0x02;
         uint32_t chunkSize;
         uint32_t unicode;
         uint32_t width;
         uint32_t height;
         int32_t xoffset;
         int32_t yoffset;
         int32_t xadvance;
         int32_t yadvance;
         uint32_t bitmapWidth;
         uint32_t bitmapheight;
     });


PACK(struct FontChunk {
         uint8_t chunkID = 0x01;
         uint32_t chunkSize;
         uint8_t fontSize = s_fontSize;
         uint8_t range = s_range;
         uint8_t padding = s_padding;
         uint8_t styleType;
         uint8_t lineHeight;
         uint8_t familyLen;
         uint8_t styleLen;
         char * family;
         char * style;
         std::vector<GlyphChunk> glyphs;
         std::vector<KerningChunk> kernings;
     });


std::ofstream& operator<<(std::ofstream& out, HeaderChunk &chunk)
{
    out.write(reinterpret_cast<char*>(&chunk), sizeof(HeaderChunk));
    return out;
}

std::ofstream& operator<<(std::ofstream& out, KerningChunk &chunk)
{
    out.write(reinterpret_cast<char*>(&chunk), sizeof(KerningChunk));
    return out;
}

std::ofstream& operator<<(std::ofstream& out, GlyphChunk &chunk)
{
    out.write(reinterpret_cast<char*>(&chunk), sizeof(GlyphChunk));
    return out;
}

std::ofstream& operator<<(std::ofstream& out, FontChunk &chunk)
{
    chunk.chunkSize = offsetof(FontChunk, family) + strlen(chunk.family) + strlen(chunk.style);

    for (GlyphChunk &glyph : chunk.glyphs)
    {
        glyph.chunkSize = sizeof(GlyphChunk);
        chunk.chunkSize += glyph.chunkSize;
    }

    for (KerningChunk &kerning : chunk.kernings)
    {
        kerning.chunkSize = sizeof(KerningChunk);
        chunk.chunkSize += kerning.chunkSize;
    }

    out.write(reinterpret_cast<char*>(&chunk), offsetof(FontChunk, family));
    out << chunk.family;
    out << chunk.style;
    for (GlyphChunk &glyph : chunk.glyphs) { out << glyph; }
    for (KerningChunk &kerning : chunk.kernings) { out << kerning; }
    return out;
}

void FontGenerator::generate(const std::string output, const std::vector<FT_Face> &faces)
{
    std::ofstream out(output, std::ios::out | std::ios::binary);

    HeaderChunk header;
    out << header;

    for(const FT_Face &face : faces)
    {
        FontChunk font;

        FT_Set_Pixel_Sizes(face, 0, s_fontSize);

        for(size_t i = 0; i < sizeof(unicode_latin1)/sizeof(uint32_t); ++i)
        {
            FT_Load_Char(face, unicode_latin1[i], FT_LOAD_RENDER);

            msdfgen::Shape shape;
            if (!loadGlyph(shape, face->glyph))
            {
                std::cerr << "Could not load glyph " << unicode_latin1[i] << std::endl;
                continue;
            }

            if (!shape.validate())
            {
                std::cerr << "The geometry of the loaded glyph " << unicode_latin1[i] << " is invalid." << std::endl;
                continue;
            }

            uint32_t glyph_width = face->glyph->bitmap.width;
            uint32_t glyph_height = face->glyph->bitmap.rows;
            int32_t glyph_left = face->glyph->bitmap_left;
            int32_t glyph_right = glyph_left + glyph_width;
            int32_t glyph_top = face->glyph->bitmap_top;
            int32_t glyph_bottom = glyph_top - glyph_height;
            uint32_t bitmapWidth = face->glyph->bitmap.width + s_range*2 + s_padding*2;
            uint32_t bitmapHeight = face->glyph->bitmap.rows + s_range*2 + s_padding*2;

            msdfgen::Vector2 frame(bitmapWidth, bitmapHeight);

            if (frame.x <= 0 || frame.y <= 0)
            {
                std::cerr << "Cannot fit the specified pixel range for glyph " << unicode_latin1[i] << std::endl;
                continue;
            }

            msdfgen::Bitmap<float> sdf(bitmapWidth, bitmapHeight);
            msdfgen::Bitmap<msdfgen::FloatRGB> msdf(bitmapWidth, bitmapHeight);

            shape.normalize();
            edgeColoringSimple(shape, 3.13);

            double l = glyph_left - (s_range + s_padding);
            double b = glyph_bottom - (s_range + s_padding);
            double r = glyph_right + (s_range + s_padding);
            double t = glyph_top + (s_range + s_padding);

            if (l >= r || b >= t)
            {
                l = 0;
                b = 0;
                r = 1;
                t = 1;
            }

            msdfgen::Vector2 dims(r-l, t-b);
            msdfgen::Vector2 scale = 1.0;
            msdfgen::Vector2 translate = 0.0;
            if (dims.x*frame.y < dims.y*frame.x)
            {
                translate.set(0.5*(frame.x/frame.y*dims.y-dims.x)-l, -b);
                scale = frame.y/dims.y;
            }
            else
            {
                translate.set(-l, 0.5*(frame.y/frame.x*dims.x-dims.y)-b);
                scale = frame.x/dims.x;
            }

            generateSDF(sdf, shape, s_range, scale, translate);
            generateMSDF(msdf, shape, s_range, scale, translate);

            // merge msdf and sdf into RGBA
            std::vector<RGBA> msdfa;
            msdfa.resize(bitmapWidth * bitmapHeight);
            for (int y = bitmapHeight - 1; y >= 0; --y)
            {
                for(int x = 0; x < bitmapWidth; ++x)
                {
                    if (y < s_padding ||
                        x < s_padding ||
                        y > bitmapHeight - s_padding - 1 ||
                        x > bitmapWidth - s_padding - 1)
                    {
                        msdfa[x + y*bitmapWidth] = {0,0,0,0};
                    }
                    else
                    {
                        msdfa[x + (bitmapHeight - 1 - y)*bitmapWidth] = {
                            static_cast<uint8_t>(std::max(0, std::min(255, int(msdf(x, y).r * 256.0f + 0.5f)))),
                            static_cast<uint8_t>(std::max(0, std::min(255, int(msdf(x, y).g * 256.0f + 0.5f)))),
                            static_cast<uint8_t>(std::max(0, std::min(255, int(msdf(x, y).b * 256.0f + 0.5f)))),
                            static_cast<uint8_t>(std::max(0, std::min(255, int(sdf(x, y) * 256.0f + 0.5f)))),
                        };
                    }
                }
            }

            std::stringstream path;
            path << face->family_name
                 << Poco::Path::separator()
                 << face->style_name
                 << Poco::Path::separator();

            Poco::File tmp(path.str());
            tmp.createDirectories();

            std::stringstream filename;
            filename << "0x"
                     << std::setfill('0')
                     << std::setw(4)
                     << std::hex
                     << unicode_latin1[i]
                     << ".png";

            std::string pathPrefix = path.str() + filename.str();

            //msdfgen::savePng(msdf, pathPrefix.c_str());
            lodepng::encode(pathPrefix.c_str(), (const unsigned char*)msdfa.data(), bitmapWidth, bitmapHeight, LCT_RGBA);

            GlyphChunk glyph;
            glyph.unicode = unicode_latin1[i];
            glyph.width = face->glyph->bitmap.width;
            glyph.height = face->glyph->bitmap.rows;
            glyph.xoffset = static_cast<int32_t>(face->glyph->bitmap_left);
            glyph.yoffset = static_cast<int32_t>(face->glyph->bitmap_top);
            glyph.xadvance = static_cast<int32_t>(face->glyph->advance.x/64.0 + 0.5);
            glyph.yadvance = static_cast<int32_t>(face->glyph->advance.y/64.0 + 0.5);
            glyph.bitmapWidth = bitmapWidth;
            glyph.bitmapheight = bitmapHeight;

            font.glyphs.emplace_back(glyph);
        }

        font.styleType = static_cast<uint8_t>(FontStyle::GetStyleByName(face->style_name).getId());
        font.lineHeight = static_cast<uint8_t>(face->size->metrics.height/64.0 + 0.5);
        font.familyLen = static_cast<uint8_t>(strlen(face->family_name));
        font.styleLen = static_cast<uint8_t>(strlen(face->style_name));
        font.family = face->family_name;
        font.style = face->style_name;

        out << font;
    }


    out.close();
}
