#ifndef SAMPLEAPP_H
#define SAMPLEAPP_H

#include <Tunis.h>

class SampleApp
{
public:
    const char *getSampleName() const;
    int getScreenWidth() const;
    int getScreenHeight() const;

    void init();
    void render(int32_t windowWidth, int32_t windowHeight, double frameTime);

private:
    std::unique_ptr<tunis::Context> ctx;
};

#endif // SAMPLEAPP_H
