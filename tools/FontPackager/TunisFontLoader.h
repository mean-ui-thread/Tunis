/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2017-2018 Mathieu-André Chiasson
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 * Disclaimer:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#ifndef TUNISFONTLOADER_H
#define TUNISFONTLOADER_H

#include <set>
#include <string>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace tunis
{

class FontLoader
{
public:

    FontLoader();
    ~FontLoader();

    void addFamily(const std::string &family);
    void addFilePattern(const std::string &pattern);

    const std::vector<FT_Face> &getFaces();

private:

    void loadFonts();
    void loadWebFonts();

    std::set<std::string> m_families;
    std::set<std::string> m_patterns;
    std::vector<FT_Face> m_faces;
    std::vector<char*> m_faceData;
    FT_Library m_library;
};

}

#endif // TUNISFONTLOADER_H
