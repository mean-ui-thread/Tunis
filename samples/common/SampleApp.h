#ifndef SAMPLEAPP_H
#define SAMPLEAPP_H
class SampleApp
{
public:
    const char *getSampleName() const;
    int getScreenWidth() const;
    int getScreenHeight() const;
    void render(double dt);
};

#endif // SAMPLEAPP_H
