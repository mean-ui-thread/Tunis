#ifndef TUNISFONTSTYLE_H
#define TUNISFONTSTYLE_H

#include <string>

class FontStyle
{
public:
    FontStyle();

    static const FontStyle Invalid;

    static const FontStyle Thin;
    static const FontStyle ThinItalic;
    static const FontStyle ExtraLight;
    static const FontStyle ExtraLightItalic;
    static const FontStyle Light;
    static const FontStyle LightItalic;
    static const FontStyle Regular;
    static const FontStyle RegularItalic;
    static const FontStyle Medium;
    static const FontStyle MediumItalic;
    static const FontStyle SemiBold;
    static const FontStyle SemiBoldItalic;
    static const FontStyle Bold;
    static const FontStyle BoldItalic;
    static const FontStyle ExtraBold;
    static const FontStyle ExtraBoldItalic;
    static const FontStyle Black;
    static const FontStyle BlackItalic;

    static const FontStyle &GetStyleByName(const std::string &name);

    inline bool operator< (const FontStyle &other) const {return m_id <  other.m_id;}
    inline bool operator<=(const FontStyle &other) const {return m_id <= other.m_id;}
    inline bool operator> (const FontStyle &other) const {return m_id >  other.m_id;}
    inline bool operator>=(const FontStyle &other) const {return m_id >= other.m_id;}
    inline bool operator==(const FontStyle &other) const {return m_id == other.m_id;}
    inline bool operator!=(const FontStyle &other) const {return m_id != other.m_id;}

    inline int getId() const {return m_id;}
    inline const std::string &getName() const {return m_name;}
    inline const std::string &getAlternateName() const {return m_alternateName;}
    inline int getThickness() const {return m_thickness;}
    inline bool isItalic() const {return m_italic;}

private:

    FontStyle(int id, const std::string &name, const std::string &alternateName, int thickness, bool italic);
    int m_id;
    std::string m_name;
    std::string m_alternateName;
    int m_thickness;
    bool m_italic;
};

#endif // TUNISFONTSTYLE_H
