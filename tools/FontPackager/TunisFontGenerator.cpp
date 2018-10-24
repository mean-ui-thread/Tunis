#include "TunisFontGenerator.h"
#include "TunisGlyphLoader.h"

#include <msdfgen/msdfgen.h>
#include <msdfgen/msdfgen-ext.h>
#include <libpng/png.h>

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

#include "TunisFonts_generated.h"


#ifdef _MSC_VER
    #define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#else
    #define PACK( __Declaration__ ) __Declaration__ __attribute__((packed, aligned(1)))
#endif

using namespace tunis;

static const uint16_t s_fontSize = 48;
static const uint8_t s_range = s_fontSize / 8;
static const uint8_t s_padding = 4;

static uint32_t unicode_latin[] = {
    // 0020-007F Basic Latin
    0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x0029, 0x002A, 0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
    0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x003A, 0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
    0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F,
    0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C, 0x005D, 0x005E, 0x005F,
    0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006E, 0x006F,
    0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077, 0x0078, 0x0079, 0x007A, 0x007B, 0x007C, 0x007D, 0x007E, 0x007F,
    // 00A0-00FF Latin-1 Supplement
    0x00A0, 0x00A1, 0x00A2, 0x00A3, 0x00A4, 0x00A5, 0x00A6, 0x00A7, 0x00A8, 0x00A9, 0x00AA, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
    0x00B0, 0x00B1, 0x00B2, 0x00B3, 0x00B4, 0x00B5, 0x00B6, 0x00B7, 0x00B8, 0x00B9, 0x00BA, 0x00BB, 0x00BC, 0x00BD, 0x00BE, 0x00BF,
    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF,
    0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x00DF,
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, 0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
    0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6, 0x00F7, 0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE, 0x00FF
};

struct RGBA
{
    uint8_t r, g, b, a;
};

static FontWeight toFontWeight(const std::string &name)
{
    if (Poco::icompare(name, "Thin") == 0 ||
        Poco::icompare(name, "Thin Italic") == 0 ||
        Poco::icompare(name, "100") == 0 ||
        Poco::icompare(name, "100italic") == 0)
        return FontWeight_Thin;

    if (Poco::icompare(name, "ExtraLight") == 0 ||
        Poco::icompare(name, "ExtraLight Italic") == 0 ||
        Poco::icompare(name, "200") == 0 ||
        Poco::icompare(name, "200italic") == 0)
        return FontWeight_ExtraLight;

    if (Poco::icompare(name, "Light") == 0 ||
        Poco::icompare(name, "Light Italic") == 0 ||
        Poco::icompare(name, "300") == 0 ||
        Poco::icompare(name, "300italic") == 0)
        return FontWeight_Light;

    if (Poco::icompare(name, "Regular") == 0 ||
        Poco::icompare(name, "Italic") == 0 ||
        Poco::icompare(name, "400") == 0 ||
        Poco::icompare(name, "400italic") == 0)
        return FontWeight_Regular;

    if (Poco::icompare(name, "Medium") == 0 ||
        Poco::icompare(name, "Medium Italic") == 0 ||
        Poco::icompare(name, "500") == 0 ||
        Poco::icompare(name, "500italic") == 0)
        return FontWeight_Medium;

    if (Poco::icompare(name, "SemiBold") == 0 ||
        Poco::icompare(name, "SemiBold Italic") == 0 ||
        Poco::icompare(name, "600") == 0 ||
        Poco::icompare(name, "600italic") == 0)
        return FontWeight_SemiBold;

    if (Poco::icompare(name, "Bold") == 0 ||
        Poco::icompare(name, "Bold Italic") == 0 ||
        Poco::icompare(name, "700") == 0 ||
        Poco::icompare(name, "700italic") == 0)
        return FontWeight_Bold;

    if (Poco::icompare(name, "ExtraBold") == 0 ||
        Poco::icompare(name, "ExtraBold Italic") == 0 ||
        Poco::icompare(name, "800") == 0 ||
        Poco::icompare(name, "800italic") == 0)
        return FontWeight_ExtraBold;

    if (Poco::icompare(name, "Black") == 0 ||
        Poco::icompare(name, "Black Italic") == 0 ||
        Poco::icompare(name, "900") == 0 ||
        Poco::icompare(name, "900italic") == 0)
        return FontWeight_Black;

    std::cerr << "Unknown style " << name << std::endl;
    return FontWeight_Invalid;
}

void FontGenerator::generate(const std::string output, const std::vector<FT_Face> &faces)
{
    flatbuffers::FlatBufferBuilder builder;

    std::vector< flatbuffers::Offset<tunis::Font> > fontBlock;

    for(const FT_Face &face : faces)
    {
        FontWeight weight = toFontWeight(face->style_name);

        std::string path =
                std::string(face->family_name) +
                Poco::Path::separator() +
                std::to_string(weight*100) +
                (face->style_flags & FT_STYLE_FLAG_ITALIC ? "italic" : "") +
                Poco::Path::separator();

        Poco::File(path).createDirectories();

        FT_Set_Pixel_Sizes(face, 0, s_fontSize);

        std::vector< flatbuffers::Offset<tunis::Glyph> > glyphBlock;

        for(size_t i = 0; i < sizeof(unicode_latin)/sizeof(uint32_t); ++i)
        {
            FT_Load_Char(face, unicode_latin[i], FT_LOAD_RENDER);

            msdfgen::Shape shape;
            if (!loadGlyph(shape, face->glyph))
            {
                std::cerr << "Could not load glyph " << unicode_latin[i] << std::endl;
                continue;
            }

            if (!shape.validate())
            {
                std::cerr << "The geometry of the loaded glyph " << unicode_latin[i] << " is invalid." << std::endl;
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
                std::cerr << "Cannot fit the specified pixel range for glyph " << unicode_latin[i] << std::endl;
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


            // Compress bitmap to png using libpng
            FILE *msdfaPNGFile = fopen((path + std::to_string(unicode_latin[i]) + ".png").c_str(), "wb");
            png_structp pPng = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
            png_infop pPngInfo = png_create_info_struct(pPng);
            png_init_io(pPng, msdfaPNGFile);
            png_set_IHDR(pPng, pPngInfo, bitmapWidth, bitmapHeight, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
            png_write_info(pPng, pPngInfo);
            std::vector<uint8_t*> rowPointers;
            rowPointers.resize(bitmapHeight);
            for (size_t y = 0; y < bitmapHeight; ++y) { rowPointers[y] = reinterpret_cast<uint8_t*>(&msdfa[y*bitmapWidth]); }
            png_write_image(pPng, &rowPointers.front());
            png_write_end(pPng, nullptr);
            png_destroy_write_struct(&pPng, &pPngInfo);
            fclose(msdfaPNGFile);

            std::vector<tunis::Kerning> kernings;
            if (FT_HAS_KERNING(face))
            {
                FT_Vector kerning;
                FT_Error error;
                for(size_t j = 0; j < sizeof(unicode_latin)/sizeof(uint32_t); ++j)
                {
                    error = FT_Get_Kerning(face,
                                           FT_Get_Char_Index(face, unicode_latin[i]),
                                           FT_Get_Char_Index(face, unicode_latin[j]),
                                           FT_KERNING_UNSCALED,
                                           &kerning);

                    if (error) continue;

                    if (kerning.x > 0)
                    {
                        kernings.emplace_back(tunis::Kerning(unicode_latin[j], kerning.x/64.0f));
                    }
                }
            }
            auto kerningVector = builder.CreateVectorOfSortedStructs(kernings.data(), kernings.size());

            tunis::GlyphBuilder glyphBuilder(builder);
            glyphBuilder.add_unicode(unicode_latin[i]);
            glyphBuilder.add_width(face->glyph->bitmap.width);
            glyphBuilder.add_height(face->glyph->bitmap.rows);
            glyphBuilder.add_xadvance(face->glyph->advance.x/64.0f);
            glyphBuilder.add_xoffset(face->glyph->bitmap_left);
            glyphBuilder.add_yoffset(face->glyph->bitmap_top);
            glyphBuilder.add_kernings(kerningVector);
            glyphBlock.push_back(glyphBuilder.Finish());
        }

        auto familyString = builder.CreateString(face->family_name);
        auto glyphVector = builder.CreateVectorOfSortedTables(glyphBlock.data(), glyphBlock.size());

        tunis::FontBuilder fontBuilder(builder);
        fontBuilder.add_family(familyString);
        fontBuilder.add_weight(weight);
        fontBuilder.add_italic(face->style_flags & FT_STYLE_FLAG_ITALIC);
        fontBuilder.add_fontSize(face->size->metrics.y_ppem);
        fontBuilder.add_padding(s_range + s_padding);
        fontBuilder.add_lineHeight(face->size->metrics.height/64.0f);
        fontBuilder.add_glyphs(glyphVector);
        fontBlock.push_back(fontBuilder.Finish());
    }

    auto fontVector = builder.CreateVector(fontBlock.data(), fontBlock.size());

    tunis::FontRepositoryBuilder fontRepositoryBuilder(builder);
    fontRepositoryBuilder.add_fonts(fontVector);
    builder.Finish(fontRepositoryBuilder.Finish());

    std::ofstream out(output, std::ios::out | std::ios::binary);

    out.write(reinterpret_cast<char*>(builder.GetBufferPointer()), builder.GetSize());

    out.close();
}
