/**
 * MIT License
 *
 * Copyright (c) 2018 Matt Chiasson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/
#include "TunisColor.h"

#include <iostream>

#if defined(_MSC_VER)
    #define TUNIS_STRCASECMP _stricmp
    #define TUNIS_STRNCASECMP _strnicmp
    #define TUNIS_SSCANF sscanf_s
#else
    #define TUNIS_STRCASECMP strcasecmp
    #define TUNIS_STRNCASECMP strncasecmp
    #define TUNIS_SSCANF sscanf
#endif

#define RGBA_COLOR(COLOR_NAME, r, g, b, a) const tunis::Color tunis::COLOR_NAME = tunis::Color(r, g, b, a)
#define RGB_COLOR(COLOR_NAME, r, g, b) RGBA_COLOR(COLOR_NAME, r, g, b, 1.0f)

RGBA_COLOR(Transparent, 0, 0, 0, 0);

/* Pink colors */
RGB_COLOR(Pink, 255, 192, 203);
RGB_COLOR(LightPink, 255, 182, 193);
RGB_COLOR(HotPink, 255, 105, 180);
RGB_COLOR(DeepPink, 255, 20, 147);
RGB_COLOR(PaleVioletRed, 219, 112, 147);
RGB_COLOR(MediumVioletRed, 199, 21, 133);

/* Red colors */
RGB_COLOR(LightSalmon, 255, 160, 122);
RGB_COLOR(Salmon, 250, 128, 114);
RGB_COLOR(DarkSalmon, 233, 150, 122);
RGB_COLOR(LightCoral, 240, 128, 128);
RGB_COLOR(IndianRed, 205, 92, 92);
RGB_COLOR(Crimson, 220, 20, 60);
RGB_COLOR(FireBrick, 178, 34, 34);
RGB_COLOR(DarkRed, 139, 0, 0);
RGB_COLOR(Red, 255, 0, 0);

/* Orange colors */
RGB_COLOR(OrangeRed, 255, 69, 0);
RGB_COLOR(Tomato, 255, 99, 71);
RGB_COLOR(Coral, 255, 127, 80);
RGB_COLOR(DarkOrange, 255, 140, 0);
RGB_COLOR(Orange, 255, 165, 0);

/* Yellow colors */
RGB_COLOR(Yellow, 255, 255, 0);
RGB_COLOR(LightYellow, 255, 255, 224);
RGB_COLOR(LemonChiffon, 255, 250, 205);
RGB_COLOR(LightGoldenrodYellow, 250, 250, 210);
RGB_COLOR(PapayaWhip, 255, 239, 213);
RGB_COLOR(Moccasin, 255, 228, 181);
RGB_COLOR(PeachPuff, 255, 218, 185);
RGB_COLOR(PaleGoldenrod, 238, 232, 170);
RGB_COLOR(Khaki, 240, 230, 140);
RGB_COLOR(DarkKhaki, 189, 183, 107);
RGB_COLOR(Gold, 255, 215, 0);

/* Brown colors */
RGB_COLOR(Cornsilk, 255, 248, 220);
RGB_COLOR(BlanchedAlmond, 255, 235, 205);
RGB_COLOR(Bisque, 255, 228, 196);
RGB_COLOR(NavajoWhite, 255, 222, 173);
RGB_COLOR(Wheat, 245, 222, 179);
RGB_COLOR(BurlyWood, 222, 184, 135);
RGB_COLOR(Tan, 210, 180, 140);
RGB_COLOR(RosyBrown, 188, 143, 143);
RGB_COLOR(SandyBrown, 244, 164, 96);
RGB_COLOR(Goldenrod, 218, 165, 32);
RGB_COLOR(DarkGoldenrod, 184, 134, 11);
RGB_COLOR(Peru, 205, 133, 63);
RGB_COLOR(Chocolate, 210, 105, 30);
RGB_COLOR(SaddleBrown, 139, 69, 19);
RGB_COLOR(Sienna, 160, 82, 45);
RGB_COLOR(Brown, 165, 42, 42);
RGB_COLOR(Maroon, 128, 0, 0);

/* Green colors */
RGB_COLOR(DarkOliveGreen, 85, 107, 47);
RGB_COLOR(Olive, 128, 128, 0);
RGB_COLOR(OliveDrab, 107, 142, 35);
RGB_COLOR(YellowGreen, 154, 205, 50);
RGB_COLOR(LimeGreen, 50, 205, 50);
RGB_COLOR(Lime, 0, 255, 0);
RGB_COLOR(LawnGreen, 124, 252, 0);
RGB_COLOR(Chartreuse, 127, 255, 0);
RGB_COLOR(GreenYellow, 173, 255, 47);
RGB_COLOR(SpringGreen, 0, 255, 127);
RGB_COLOR(MediumSpringGreen, 0, 250, 154);
RGB_COLOR(LightGreen, 144, 238, 144);
RGB_COLOR(PaleGreen, 152, 251, 152);
RGB_COLOR(DarkSeaGreen, 143, 188, 143);
RGB_COLOR(MediumSeaGreen, 60, 179, 113);
RGB_COLOR(SeaGreen, 46, 139, 87);
RGB_COLOR(ForestGreen, 34, 139, 34);
RGB_COLOR(Green, 0, 128, 0);
RGB_COLOR(DarkGreen, 0, 100, 0);

/* Cyan colors */
RGB_COLOR(MediumAquamarine, 102, 205, 170);
RGB_COLOR(Aqua, 0, 255, 255);
RGB_COLOR(Cyan, 0, 255, 255);
RGB_COLOR(LightCyan, 224, 255, 255);
RGB_COLOR(PaleTurquoise, 175, 238, 238);
RGB_COLOR(Aquamarine, 127, 255, 212);
RGB_COLOR(Turquoise, 64, 224, 208);
RGB_COLOR(MediumTurquoise, 72, 209, 204);
RGB_COLOR(DarkTurquoise, 0, 206, 209);
RGB_COLOR(LightSeaGreen, 32, 178, 170);
RGB_COLOR(CadetBlue, 95, 158, 160);
RGB_COLOR(DarkCyan, 0, 139, 139);
RGB_COLOR(Teal, 0, 128, 128);

/* Blue colors */
RGB_COLOR(LightSteelBlue, 176, 196, 222);
RGB_COLOR(PowderBlue, 176, 224, 230);
RGB_COLOR(LightBlue, 173, 216, 230);
RGB_COLOR(SkyBlue, 135, 206, 235);
RGB_COLOR(LightSkyBlue, 135, 206, 250);
RGB_COLOR(DeepSkyBlue, 0, 191, 255);
RGB_COLOR(DodgerBlue, 30, 144, 255);
RGB_COLOR(CornflowerBlue, 100, 149, 237);
RGB_COLOR(SteelBlue, 70, 130, 180);
RGB_COLOR(RoyalBlue, 65, 105, 225);
RGB_COLOR(Blue, 0, 0, 255);
RGB_COLOR(MediumBlue, 0, 0, 205);
RGB_COLOR(DarkBlue, 0, 0, 139);
RGB_COLOR(Navy, 0, 0, 128);
RGB_COLOR(MidnightBlue, 25, 25, 112);

/* Purple/Violet/Magenta colors */
RGB_COLOR(Lavender, 230, 230, 250);
RGB_COLOR(Thistle, 216, 191, 216);
RGB_COLOR(Plum, 221, 160, 221);
RGB_COLOR(Violet, 238, 130, 238);
RGB_COLOR(Orchid, 218, 112, 214);
RGB_COLOR(Fuchsia, 255, 0, 255);
RGB_COLOR(Magenta, 255, 0, 255);
RGB_COLOR(MediumOrchid, 186, 85, 211);
RGB_COLOR(MediumPurple, 147, 112, 219);
RGB_COLOR(BlueViolet, 138, 43, 226);
RGB_COLOR(DarkViolet, 148, 0, 211);
RGB_COLOR(DarkOrchid, 153, 50, 204);
RGB_COLOR(DarkMagenta, 139, 0, 139);
RGB_COLOR(Purple, 128, 0, 128);
RGB_COLOR(Indigo, 75, 0, 130);
RGB_COLOR(DarkSlateBlue, 72, 61, 139);
RGB_COLOR(RebeccaPurple, 102, 51, 153);
RGB_COLOR(SlateBlue, 106, 90, 205);
RGB_COLOR(MediumSlateBlue, 123, 104, 238);

/* White colors */
RGB_COLOR(White, 255, 255, 255);
RGB_COLOR(Snow, 255, 250, 250);
RGB_COLOR(Honeydew, 240, 255, 240);
RGB_COLOR(MintCream, 245, 255, 250);
RGB_COLOR(Azure, 240, 255, 255);
RGB_COLOR(AliceBlue, 240, 248, 255);
RGB_COLOR(GhostWhite, 248, 248, 255);
RGB_COLOR(WhiteSmoke, 245, 245, 245);
RGB_COLOR(Seashell, 255, 245, 238);
RGB_COLOR(Beige, 245, 245, 220);
RGB_COLOR(OldLace, 253, 245, 230);
RGB_COLOR(FloralWhite, 255, 250, 240);
RGB_COLOR(Ivory, 255, 255, 240);
RGB_COLOR(AntiqueWhite, 250, 235, 215);
RGB_COLOR(Linen, 250, 240, 230);
RGB_COLOR(LavenderBlush, 255, 240, 245);
RGB_COLOR(MistyRose, 255, 228, 225);

/* Gray/Black colors */
RGB_COLOR(Gainsboro, 220, 220, 220);
RGB_COLOR(LightGrey, 211, 211, 211);
RGB_COLOR(Silver, 192, 192, 192);
RGB_COLOR(DarkGray, 169, 169, 169);
RGB_COLOR(Gray, 128, 128, 128);
RGB_COLOR(DimGray, 105, 105, 105);
RGB_COLOR(LightSlateGray, 119, 136, 153);
RGB_COLOR(SlateGray, 112, 128, 144);
RGB_COLOR(DarkSlateGray, 47, 79, 79);
RGB_COLOR(Black, 0, 0, 0);

tunis::Color::Color(const char* colorName)
{
    if (colorName[0] == '#')
    {
        tunis::Color color;
        uint32_t word = static_cast<uint32_t>(strtol(&colorName[1], nullptr, 16));
        size_t colorNameLength = strlen(&colorName[1]);

        switch(colorNameLength)
        {
            case 3: // "#RGB"
                b = ((word & 0x00F) >> 0) * 17;
                g = ((word & 0x0F0) >> 4) * 17;
                r = ((word & 0xF00) >> 8) * 17;
                a = 255;
                return;
            case 4: // "#ARGB"
                b = ((word & 0x000F) >> 0) * 17;
                g = ((word & 0x00F0) >> 4) * 17;
                r = ((word & 0x0F00) >> 8) * 17;
                a = ((word & 0xF000) >> 12) * 17;
                return;
            case 6: // "#RRGGBB"
                b = ((word & 0x0000FF) >> 0);
                g = ((word & 0x00FF00) >> 8);
                r = ((word & 0xFF0000) >> 16);
                a = 255;
                return;
            case 8: // "#AARRGGBB"
                /* A R G B */
                b = ((word & 0x000000FF) >> 0);
                g = ((word & 0x0000FF00) >> 8);
                r = ((word & 0x00FF0000) >> 16);
                a = ((word & 0xFF000000) >> 24);
                return;
            default: // parse error
                r=0;
                g=0;
                b=0;
                a=0;
                return;
        }
    }
    else
    {
        uint32_t red, green, blue;
        float alpha;

        if (TUNIS_SSCANF(colorName, "rgba(%u, %u, %u, %f)", &red, &green, &blue, &alpha) == 4)
        {
            r = static_cast<uint8_t>(red);
            g = static_cast<uint8_t>(green);
            b = static_cast<uint8_t>(blue);
            a = static_cast<uint8_t>(alpha / 255.0f);
            return;
        }

        if (TUNIS_SSCANF(colorName, "rgb(%u, %u, %u)", &red, &green, &blue) == 3)
        {
            r = static_cast<uint8_t>(red);
            g = static_cast<uint8_t>(green);
            b = static_cast<uint8_t>(blue);
            a = 255;
            return;
        }
    }

    #define RETURN_IF_MATCH(COLOR_NAME) do { if(TUNIS_STRCASECMP(colorName, #COLOR_NAME) == 0) { r=COLOR_NAME.r; g=COLOR_NAME.g; b=COLOR_NAME.b; a=COLOR_NAME.a;  return; } } while((void)0, 0)

    RETURN_IF_MATCH(Transparent);

    /* Pink colors */
    RETURN_IF_MATCH(Pink);
    RETURN_IF_MATCH(LightPink);
    RETURN_IF_MATCH(HotPink);
    RETURN_IF_MATCH(DeepPink);
    RETURN_IF_MATCH(PaleVioletRed);
    RETURN_IF_MATCH(MediumVioletRed);

    /* Red colors */
    RETURN_IF_MATCH(LightSalmon);
    RETURN_IF_MATCH(Salmon);
    RETURN_IF_MATCH(DarkSalmon);
    RETURN_IF_MATCH(LightCoral);
    RETURN_IF_MATCH(IndianRed);
    RETURN_IF_MATCH(Crimson);
    RETURN_IF_MATCH(FireBrick);
    RETURN_IF_MATCH(DarkRed);
    RETURN_IF_MATCH(Red);

    /* Orange colors */
    RETURN_IF_MATCH(OrangeRed);
    RETURN_IF_MATCH(Tomato);
    RETURN_IF_MATCH(Coral);
    RETURN_IF_MATCH(DarkOrange);
    RETURN_IF_MATCH(Orange);

    /* Yellow colors */
    RETURN_IF_MATCH(Yellow);
    RETURN_IF_MATCH(LightYellow);
    RETURN_IF_MATCH(LemonChiffon);
    RETURN_IF_MATCH(LightGoldenrodYellow);
    RETURN_IF_MATCH(PapayaWhip);
    RETURN_IF_MATCH(Moccasin);
    RETURN_IF_MATCH(PeachPuff);
    RETURN_IF_MATCH(PaleGoldenrod);
    RETURN_IF_MATCH(Khaki);
    RETURN_IF_MATCH(DarkKhaki);
    RETURN_IF_MATCH(Gold);

    /* Brown colors */
    RETURN_IF_MATCH(Cornsilk);
    RETURN_IF_MATCH(BlanchedAlmond);
    RETURN_IF_MATCH(Bisque);
    RETURN_IF_MATCH(NavajoWhite);
    RETURN_IF_MATCH(Wheat);
    RETURN_IF_MATCH(BurlyWood);
    RETURN_IF_MATCH(Tan);
    RETURN_IF_MATCH(RosyBrown);
    RETURN_IF_MATCH(SandyBrown);
    RETURN_IF_MATCH(Goldenrod);
    RETURN_IF_MATCH(DarkGoldenrod);
    RETURN_IF_MATCH(Peru);
    RETURN_IF_MATCH(Chocolate);
    RETURN_IF_MATCH(SaddleBrown);
    RETURN_IF_MATCH(Sienna);
    RETURN_IF_MATCH(Brown);
    RETURN_IF_MATCH(Maroon);

    /* Green colors */
    RETURN_IF_MATCH(DarkOliveGreen);
    RETURN_IF_MATCH(Olive);
    RETURN_IF_MATCH(OliveDrab);
    RETURN_IF_MATCH(YellowGreen);
    RETURN_IF_MATCH(LimeGreen);
    RETURN_IF_MATCH(Lime);
    RETURN_IF_MATCH(LawnGreen);
    RETURN_IF_MATCH(Chartreuse);
    RETURN_IF_MATCH(GreenYellow);
    RETURN_IF_MATCH(SpringGreen);
    RETURN_IF_MATCH(MediumSpringGreen);
    RETURN_IF_MATCH(LightGreen);
    RETURN_IF_MATCH(PaleGreen);
    RETURN_IF_MATCH(DarkSeaGreen);
    RETURN_IF_MATCH(MediumSeaGreen);
    RETURN_IF_MATCH(SeaGreen);
    RETURN_IF_MATCH(ForestGreen);
    RETURN_IF_MATCH(Green);
    RETURN_IF_MATCH(DarkGreen);

    /* Cyan colors */
    RETURN_IF_MATCH(MediumAquamarine);
    RETURN_IF_MATCH(Aqua);
    RETURN_IF_MATCH(Cyan);
    RETURN_IF_MATCH(LightCyan);
    RETURN_IF_MATCH(PaleTurquoise);
    RETURN_IF_MATCH(Aquamarine);
    RETURN_IF_MATCH(Turquoise);
    RETURN_IF_MATCH(MediumTurquoise);
    RETURN_IF_MATCH(DarkTurquoise);
    RETURN_IF_MATCH(LightSeaGreen);
    RETURN_IF_MATCH(CadetBlue);
    RETURN_IF_MATCH(DarkCyan);
    RETURN_IF_MATCH(Teal);

    /* Blue colors */
    RETURN_IF_MATCH(LightSteelBlue);
    RETURN_IF_MATCH(PowderBlue);
    RETURN_IF_MATCH(LightBlue);
    RETURN_IF_MATCH(SkyBlue);
    RETURN_IF_MATCH(LightSkyBlue);
    RETURN_IF_MATCH(DeepSkyBlue);
    RETURN_IF_MATCH(DodgerBlue);
    RETURN_IF_MATCH(CornflowerBlue);
    RETURN_IF_MATCH(SteelBlue);
    RETURN_IF_MATCH(RoyalBlue);
    RETURN_IF_MATCH(Blue);
    RETURN_IF_MATCH(MediumBlue);
    RETURN_IF_MATCH(DarkBlue);
    RETURN_IF_MATCH(Navy);
    RETURN_IF_MATCH(MidnightBlue);

    /* Purple/Violet/Magenta colors */
    RETURN_IF_MATCH(Lavender);
    RETURN_IF_MATCH(Thistle);
    RETURN_IF_MATCH(Plum);
    RETURN_IF_MATCH(Violet);
    RETURN_IF_MATCH(Orchid);
    RETURN_IF_MATCH(Fuchsia);
    RETURN_IF_MATCH(Magenta);
    RETURN_IF_MATCH(MediumOrchid);
    RETURN_IF_MATCH(MediumPurple);
    RETURN_IF_MATCH(BlueViolet);
    RETURN_IF_MATCH(DarkViolet);
    RETURN_IF_MATCH(DarkOrchid);
    RETURN_IF_MATCH(DarkMagenta);
    RETURN_IF_MATCH(Purple);
    RETURN_IF_MATCH(Indigo);
    RETURN_IF_MATCH(DarkSlateBlue);
    RETURN_IF_MATCH(RebeccaPurple);
    RETURN_IF_MATCH(SlateBlue);
    RETURN_IF_MATCH(MediumSlateBlue);

    /* White colors */
    RETURN_IF_MATCH(White);
    RETURN_IF_MATCH(Snow);
    RETURN_IF_MATCH(Honeydew);
    RETURN_IF_MATCH(MintCream);
    RETURN_IF_MATCH(Azure);
    RETURN_IF_MATCH(AliceBlue);
    RETURN_IF_MATCH(GhostWhite);
    RETURN_IF_MATCH(WhiteSmoke);
    RETURN_IF_MATCH(Seashell);
    RETURN_IF_MATCH(Beige);
    RETURN_IF_MATCH(OldLace);
    RETURN_IF_MATCH(FloralWhite);
    RETURN_IF_MATCH(Ivory);
    RETURN_IF_MATCH(AntiqueWhite);
    RETURN_IF_MATCH(Linen);
    RETURN_IF_MATCH(LavenderBlush);
    RETURN_IF_MATCH(MistyRose);

    /* Gray/Black colors */
    RETURN_IF_MATCH(Gainsboro);
    RETURN_IF_MATCH(LightGrey);
    RETURN_IF_MATCH(Silver);
    RETURN_IF_MATCH(DarkGray);
    RETURN_IF_MATCH(Gray);
    RETURN_IF_MATCH(DimGray);
    RETURN_IF_MATCH(LightSlateGray);
    RETURN_IF_MATCH(SlateGray);
    RETURN_IF_MATCH(DarkSlateGray);
    RETURN_IF_MATCH(Black);

    r=0;
    g=0;
    b=0;
    a=0;
}
