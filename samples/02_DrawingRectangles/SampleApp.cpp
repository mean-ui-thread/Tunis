#include "SampleApp.h"

#include <stdio.h>

#include <easy/profiler.h>

using namespace tunis::color;

const char *SampleApp::getSampleName() { return "02_DrawingRectangles"; }
int SampleApp::getScreenWidth() { return 320; }
int SampleApp::getScreenHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Rectangular_shape_example
 */
void SampleApp::render(int32_t windowWidth, int32_t windowHeight, double frameTime)
{
    EASY_FUNCTION()

    ctx.beginFrame(0, 0, windowWidth, windowHeight);

    ctx.fillRect(25, 25, 100, 100);
    ctx.clearRect(45, 45, 60, 60);
    ctx.strokeRect(50, 50, 50, 50);

    ctx.endFrame();
}
