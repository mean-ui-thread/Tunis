#include "TunisFontDef.h"
#include "TunisUtil.h"

#include <cstring>
#include <iostream>
#include <vector>

namespace tunis
{
    inline FontDef::FontDef() :
        weight(4),
        italic(false),
        fontSize(0)
    {
    }

    inline FontDef::FontDef(const char *fontStr) :
        weight(4),
        italic(false),
        fontSize(0)
    {

        std::string str(fontStr);

        std::size_t pxPos = str.find("px ");

        if (pxPos == std::string::npos)
        {
            std::cerr << "Invalid font definition '" << fontStr << "'" << std::endl;
            return;
        }

        setFamily(str.substr(pxPos+3));

        char * p = strtok(&str.substr(0, pxPos).front(), " ");
        std::vector<char*> styles;
        while (p)
        {
            styles.push_back(p);
            p = strtok(nullptr, " ");
        }

        switch (styles.size())
        {
            case 1:
                setFontSize(styles[0]);
                break;

            case 2:
                setStyle(styles[0]);
                setFontSize(styles[1]);
                break;

            case 3:
                setStyle(styles[0]);
                setStyle(styles[1]);
                setFontSize(styles[2]);
                break;

            case 4:
                setStyle(styles[0]);
                setStyle(styles[1]);
                setStyle(styles[2]);
                setFontSize(styles[3]);
                break;

            default:
                std::cerr << "Invalid font definition '" << fontStr << "'" << std::endl;
                break;
        }
    }

    inline void FontDef::setStyle(std::string token)
    {
        if (TUNIS_STRNCASECMP(token.c_str(), "italic", 6) == 0)
        {
            italic = true;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "thin", 4) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "100", 3) == 0)
        {
            weight = 1;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "extralight", 10) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "200", 3) == 0)
        {
            weight = 2;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "light", 5) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "300", 3) == 0)
        {
            weight = 3;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "regular", 7) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "400", 3) == 0)
        {
            weight = 4;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "medium", 6) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "500", 3) == 0)
        {
            weight = 5;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "semibold", 8) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "600", 3) == 0)
        {
            weight = 6;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "bold", 4) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "700", 3) == 0)
        {
            weight = 7;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "extrabold", 9) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "800", 3) == 0)
        {
            weight = 8;
        }
        else if (TUNIS_STRNCASECMP(token.c_str(), "black", 5) == 0 ||
                 TUNIS_STRNCASECMP(token.c_str(), "900", 3) == 0)
        {
            weight = 9;
        }
        else
        {
            std::cerr << "Unknown/Unsupported font style/weight '" << token << "'" << std::endl;

        }
    }

    inline void FontDef::setFontSize(std::string token)
    {
        fontSize = atoi(token.c_str());
    }

    inline void FontDef::setFamily(std::string token)
    {
        family = token;
    }

}

