#ifndef TUNISFONTGENERATOR_H
#define TUNISFONTGENERATOR_H

#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace tunis
{

class FontGenerator
{
public:

    void generate(const std::string output, const std::vector<FT_Face> &faces);
};

}

#endif // TUNISFONTGENERATOR_H
