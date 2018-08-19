#include "SampleApp.h"

#include <stdio.h>

using namespace tunis::color;

const char *SampleApp::getSampleName() { return "01_SimpleExample"; }
int SampleApp::getScreenWidth() { return 320; }
int SampleApp::getScreenHeight() { return 200; }

/*!
 * Based of https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Basic_usage#A_simple_example
 */
void SampleApp::render(int32_t windowWidth, int32_t windowHeight, double frameTime)
{
    ctx.beginFrame(0, 0, windowWidth, windowHeight);

    ctx.fillStyle = rgb(200, 0, 0);
    ctx.fillRect(10, 10, 50, 50);

    ctx.fillStyle = rgba(0, 0, 200, 0.5);
    ctx.fillRect(30, 30, 50, 50);

    ctx.endFrame();
}
