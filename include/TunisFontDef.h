#ifndef TUNISFONTDEF_H
#define TUNISFONTDEF_H

#include <string>

namespace tunis
{
    class FontDef
    {
    public:
        FontDef();
        FontDef(const char *fontStr);

        void setStyle(std::string token);
        void setFontSize(std::string token);
        void setFamily(std::string token);

        std::string family;
        uint8_t weight;
        bool italic;
        int32_t fontSize;
    };
}

#include <TunisFontDef.inl>

#endif // TUNISFONTDEF_H
