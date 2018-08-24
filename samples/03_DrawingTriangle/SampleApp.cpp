#include "SampleApp.h"

#include <stdio.h>

using namespace tunis::color;

const char *SampleApp::getSampleName() { return "03_DrawingTriangle"; }
int SampleApp::getScreenWidth() { return 320; }
int SampleApp::getScreenHeight() { return 200; }

/*!
 * Based of https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Drawing_a_triangle
 */
void SampleApp::render(int32_t windowWidth, int32_t windowHeight, double frameTime)
{
    ctx.beginFrame(0, 0, windowWidth, windowHeight);

    ctx.beginPath();
    ctx.moveTo(75, 50);
    ctx.lineTo(100, 75);
    ctx.lineTo(100, 25);
    ctx.fill();

    ctx.endFrame();
}
