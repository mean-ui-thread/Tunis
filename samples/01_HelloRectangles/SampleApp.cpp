#include "SampleApp.h"

#include <stdio.h>

using namespace tunis::color;

const char *SampleApp::getSampleName() const { return "01_HelloRectangles"; }
int SampleApp::getScreenWidth() const { return 1280; }
int SampleApp::getScreenHeight() const { return 720; }

void SampleApp::init()
{
    ctx.reset(new tunis::Context());

    ctx->setBackgroundColor(tunis::color::White);
}


void SampleApp::render(int32_t windowWidth, int32_t windowHeight, double frameTime)
{
    ctx->beginFrame(0, 0, windowWidth, windowHeight);

    ctx->fillStyle = rgb(200, 0, 0);
    ctx->fillRect(10, 10, 50, 50);

    ctx->fillStyle = rgba(0, 0, 200, 0.5);
    ctx->fillRect(30, 30, 50, 50);

    ctx->endFrame();
}
