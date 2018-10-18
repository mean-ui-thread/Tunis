#ifndef TUNISGLYPH_H
#define TUNISGLYPH_H

#include <cinttypes>

namespace tunis
{

    struct Glyph
    {
        unsigned int unicode;
        float width = 0.0f;
        float height = 0.0f;
        float xoffset = 0.0f;
        float yoffset = 0.0f;
        float xadvance = 0.0f;
        float yadvance = 0.0f;
    };


}


#endif // TUNISGLYPH_H
