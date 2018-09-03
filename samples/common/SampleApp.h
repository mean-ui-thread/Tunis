#ifndef SAMPLEAPP_H
#define SAMPLEAPP_H

#include <Tunis.h>

using namespace tunis;

class SampleApp
{
public:
    static const char *getSampleName();
    static int getWindowWidth();
    static int getWindowHeight();

    void render(double frameTime);

    tunis::Context ctx;
};

#endif // SAMPLEAPP_H
