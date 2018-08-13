#include "SampleApp.h"

#include <stdio.h>

const char *SampleApp::getSampleName() const { return "01_HelloTriangle"; }
int SampleApp::getScreenWidth() const { return 1280; }
int SampleApp::getScreenHeight() const { return 720; }

void SampleApp::init()
{
    ctx.reset(new tunis::Context());
}


void SampleApp::render(int32_t windowWidth, int32_t windowHeight, double frameTime)
{
    ctx->beginFrame(windowWidth, windowHeight);


    ctx->endFrame();
}
