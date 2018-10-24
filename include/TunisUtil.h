#ifndef TUNISUTIL_H
#define TUNISUTIL_H

#if defined(_MSC_VER)
    #define TUNIS_STRCASECMP _stricmp
    #define TUNIS_STRNCASECMP _strnicmp
    #define TUNIS_SSCANF sscanf_s
#else
    #define TUNIS_STRCASECMP strcasecmp
    #define TUNIS_STRNCASECMP strncasecmp
    #define TUNIS_SSCANF sscanf
#endif

#endif // TUNISUTIL_H
