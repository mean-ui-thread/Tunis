#ifndef TUNISCOLOR_H
#define TUNISCOLOR_H

#include <cstdint>

#include <TunisTypes.h>

namespace tunis
{
class Color : public glm::u8vec4
{
public:
    Color() : glm::u8vec4() {}
    Color(uint8_t r, uint8_t g, uint8_t b, float a = 1.0f) : glm::u8vec4(r, g, b, a*255) {}
    Color(const char* colorName);
};

inline Color rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return Color(r, g, b, 1.0f);
}

inline Color rgba(uint8_t r, uint8_t g, uint8_t b, float a)
{
    return Color(r, g, b, a);
}


extern const Color Transparent;

/* Pink colors */
extern const Color Pink;
extern const Color LightPink;
extern const Color HotPink;
extern const Color DeepPink;
extern const Color PaleVioletRed;
extern const Color MediumVioletRed;

/* Red colors */
extern const Color LightSalmon;
extern const Color Salmon;
extern const Color DarkSalmon;
extern const Color LightCoral;
extern const Color IndianRed;
extern const Color Crimson;
extern const Color FireBrick;
extern const Color DarkRed;
extern const Color Red;

/* Orange colors */
extern const Color OrangeRed;
extern const Color Tomato;
extern const Color Coral;
extern const Color DarkOrange;
extern const Color Orange;

/* Yellow colors */
extern const Color Yellow;
extern const Color LightYellow;
extern const Color LemonChiffon;
extern const Color LightGoldenrodYellow;
extern const Color PapayaWhip;
extern const Color Moccasin;
extern const Color PeachPuff;
extern const Color PaleGoldenrod;
extern const Color Khaki;
extern const Color DarkKhaki;
extern const Color Gold;

/* Brown colors */
extern const Color Cornsilk;
extern const Color BlanchedAlmond;
extern const Color Bisque;
extern const Color NavajoWhite;
extern const Color Wheat;
extern const Color BurlyWood;
extern const Color Tan;
extern const Color RosyBrown;
extern const Color SandyBrown;
extern const Color Goldenrod;
extern const Color DarkGoldenrod;
extern const Color Peru;
extern const Color Chocolate;
extern const Color SaddleBrown;
extern const Color Sienna;
extern const Color Brown;
extern const Color Maroon;

/* Green colors */
extern const Color DarkOliveGreen;
extern const Color Olive;
extern const Color OliveDrab;
extern const Color YellowGreen;
extern const Color LimeGreen;
extern const Color Lime;
extern const Color LawnGreen;
extern const Color Chartreuse;
extern const Color GreenYellow;
extern const Color SpringGreen;
extern const Color MediumSpringGreen;
extern const Color LightGreen;
extern const Color PaleGreen;
extern const Color DarkSeaGreen;
extern const Color MediumSeaGreen;
extern const Color SeaGreen;
extern const Color ForestGreen;
extern const Color Green;
extern const Color DarkGreen;

/* Cyan colors */
extern const Color MediumAquamarine;
extern const Color Aqua;
extern const Color Cyan;
extern const Color LightCyan;
extern const Color PaleTurquoise;
extern const Color Aquamarine;
extern const Color Turquoise;
extern const Color MediumTurquoise;
extern const Color DarkTurquoise;
extern const Color LightSeaGreen;
extern const Color CadetBlue;
extern const Color DarkCyan;
extern const Color Teal;

/* Blue colors */
extern const Color LightSteelBlue;
extern const Color PowderBlue;
extern const Color LightBlue;
extern const Color SkyBlue;
extern const Color LightSkyBlue;
extern const Color DeepSkyBlue;
extern const Color DodgerBlue;
extern const Color CornflowerBlue;
extern const Color SteelBlue;
extern const Color RoyalBlue;
extern const Color Blue;
extern const Color MediumBlue;
extern const Color DarkBlue;
extern const Color Navy;
extern const Color MidnightBlue;

/* Purple/Violet/Magenta colors */
extern const Color Lavender;
extern const Color Thistle;
extern const Color Plum;
extern const Color Violet;
extern const Color Orchid;
extern const Color Fuchsia;
extern const Color Magenta;
extern const Color MediumOrchid;
extern const Color MediumPurple;
extern const Color BlueViolet;
extern const Color DarkViolet;
extern const Color DarkOrchid;
extern const Color DarkMagenta;
extern const Color Purple;
extern const Color Indigo;
extern const Color DarkSlateBlue;
extern const Color RebeccaPurple;
extern const Color SlateBlue;
extern const Color MediumSlateBlue;

/* White colors */
extern const Color White;
extern const Color Snow;
extern const Color Honeydew;
extern const Color MintCream;
extern const Color Azure;
extern const Color AliceBlue;
extern const Color GhostWhite;
extern const Color WhiteSmoke;
extern const Color Seashell;
extern const Color Beige;
extern const Color OldLace;
extern const Color FloralWhite;
extern const Color Ivory;
extern const Color AntiqueWhite;
extern const Color Linen;
extern const Color LavenderBlush;
extern const Color MistyRose;

/* Gray/Black colors */
extern const Color Gainsboro;
extern const Color LightGrey;
extern const Color Silver;
extern const Color DarkGray;
extern const Color Gray;
extern const Color DimGray;
extern const Color LightSlateGray;
extern const Color SlateGray;
extern const Color DarkSlateGray;
extern const Color Black;

}

#endif // TUNISCOLOR_H
