#include "TunisFontStyle.h"

#include <Poco/String.h>

const FontStyle FontStyle::Invalid          = FontStyle(0,  "Invalid",            "",            0, false);
const FontStyle FontStyle::Thin             = FontStyle(1,  "Thin",               "100",       100, false);
const FontStyle FontStyle::ThinItalic       = FontStyle(2,  "Thin Italic",        "100italic", 100, true);
const FontStyle FontStyle::ExtraLight       = FontStyle(3,  "Extra-Light",        "200",       200, false);
const FontStyle FontStyle::ExtraLightItalic = FontStyle(4,  "Extra-Light Italic", "200italic", 200, true);
const FontStyle FontStyle::Light            = FontStyle(5,  "Light",              "300",       300, false);
const FontStyle FontStyle::LightItalic      = FontStyle(6,  "Light Italic",       "300italic", 300, true);
const FontStyle FontStyle::Regular          = FontStyle(7,  "Regular",            "regular",   400, false);
const FontStyle FontStyle::RegularItalic    = FontStyle(8,  "Regular Italic",     "italic",    400, true);
const FontStyle FontStyle::Medium           = FontStyle(9,  "Medium",             "500",       500, false);
const FontStyle FontStyle::MediumItalic     = FontStyle(10, "Medium Italic",      "500italic", 500, true);
const FontStyle FontStyle::SemiBold         = FontStyle(11, "Semi-Bold",          "600",       600, false);
const FontStyle FontStyle::SemiBoldItalic   = FontStyle(12, "Semi-Bold Italic",   "600italic", 600, true);
const FontStyle FontStyle::Bold             = FontStyle(13, "Bold",               "700",       700, false);
const FontStyle FontStyle::BoldItalic       = FontStyle(14, "Bold Italic",        "700italic", 700, true);
const FontStyle FontStyle::ExtraBold        = FontStyle(15, "Extra-Bold",         "800",       800, false);
const FontStyle FontStyle::ExtraBoldItalic  = FontStyle(16, "Extra-Bold Italic",  "800italic", 800, true);
const FontStyle FontStyle::Black            = FontStyle(17, "Black",              "900",       900, false);
const FontStyle FontStyle::BlackItalic      = FontStyle(18, "Black Italic",       "900italic", 900, true);

FontStyle::FontStyle()
{
    *this = Invalid;
}

const FontStyle &FontStyle::GetStyleByName(const std::string &name)
{
    if (Poco::icompare(name, Thin            .m_name) == 0 || Poco::icompare(name, Thin.getAlternateName()            ) == 0) return Thin            ;
    if (Poco::icompare(name, ThinItalic      .m_name) == 0 || Poco::icompare(name, ThinItalic.getAlternateName()      ) == 0) return ThinItalic      ;
    if (Poco::icompare(name, ExtraLight      .m_name) == 0 || Poco::icompare(name, ExtraLight.getAlternateName()      ) == 0) return ExtraLight      ;
    if (Poco::icompare(name, ExtraLightItalic.m_name) == 0 || Poco::icompare(name, ExtraLightItalic.getAlternateName()) == 0) return ExtraLightItalic;
    if (Poco::icompare(name, Light           .m_name) == 0 || Poco::icompare(name, Light.getAlternateName()           ) == 0) return Light           ;
    if (Poco::icompare(name, LightItalic     .m_name) == 0 || Poco::icompare(name, LightItalic.getAlternateName()     ) == 0) return LightItalic     ;
    if (Poco::icompare(name, Regular         .m_name) == 0 || Poco::icompare(name, Regular.getAlternateName()         ) == 0) return Regular         ;
    if (Poco::icompare(name, RegularItalic   .m_name) == 0 || Poco::icompare(name, RegularItalic.getAlternateName()   ) == 0) return RegularItalic   ;
    if (Poco::icompare(name, Medium          .m_name) == 0 || Poco::icompare(name, Medium.getAlternateName()          ) == 0) return Medium          ;
    if (Poco::icompare(name, MediumItalic    .m_name) == 0 || Poco::icompare(name, MediumItalic.getAlternateName()    ) == 0) return MediumItalic    ;
    if (Poco::icompare(name, SemiBold        .m_name) == 0 || Poco::icompare(name, SemiBold.getAlternateName()        ) == 0) return SemiBold        ;
    if (Poco::icompare(name, SemiBoldItalic  .m_name) == 0 || Poco::icompare(name, SemiBoldItalic.getAlternateName()  ) == 0) return SemiBoldItalic  ;
    if (Poco::icompare(name, Bold            .m_name) == 0 || Poco::icompare(name, Bold.getAlternateName()            ) == 0) return Bold            ;
    if (Poco::icompare(name, BoldItalic      .m_name) == 0 || Poco::icompare(name, BoldItalic.getAlternateName()      ) == 0) return BoldItalic      ;
    if (Poco::icompare(name, ExtraBold       .m_name) == 0 || Poco::icompare(name, ExtraBold.getAlternateName()       ) == 0) return ExtraBold       ;
    if (Poco::icompare(name, ExtraBoldItalic .m_name) == 0 || Poco::icompare(name, ExtraBoldItalic.getAlternateName() ) == 0) return ExtraBoldItalic ;
    if (Poco::icompare(name, Black           .m_name) == 0 || Poco::icompare(name, Black.getAlternateName()           ) == 0) return Black           ;
    if (Poco::icompare(name, BlackItalic     .m_name) == 0 || Poco::icompare(name, BlackItalic.getAlternateName()     ) == 0) return BlackItalic     ;

    return Invalid;
}

FontStyle::FontStyle(int id, const std::string &name, const std::string &alternateName, int thickness, bool italic) :
    m_id(id),
    m_name(name),
    m_alternateName(alternateName),
    m_thickness(thickness),
    m_italic(italic)
{
}
