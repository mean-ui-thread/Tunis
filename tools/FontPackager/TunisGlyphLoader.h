#ifndef TUNISGLYPHLOADER_H
#define TUNISGLYPHLOADER_H

#include <msdfgen/core/Shape.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace tunis
{
    bool loadGlyph(msdfgen::Shape &output, FT_Face face, FT_ULong unicode);
    bool loadGlyph(msdfgen::Shape &output, FT_GlyphSlot glyph);
    bool getKerning(double &output, FT_Face *face, int unicode1, int unicode2);
}


#endif // TUNISGLYPHLOADER_H
