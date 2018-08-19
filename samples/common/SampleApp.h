#ifndef SAMPLEAPP_H
#define SAMPLEAPP_H

#include <Tunis.h>

class SampleApp
{
public:
    static const char *getSampleName();
    static int getScreenWidth();
    static int getScreenHeight();

    void render(int32_t windowWidth, int32_t windowHeight, double frameTime);

    tunis::Context ctx;
};

#endif // SAMPLEAPP_H
