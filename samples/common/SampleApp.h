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
    void render(double dt);

private:
    std::unique_ptr<tunis::Context> ctx;
};

#endif // SAMPLEAPP_H
