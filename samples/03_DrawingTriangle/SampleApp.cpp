#include "SampleApp.h"

#include <stdio.h>

#include <easy/profiler.h>

using namespace tunis::color;

const char *SampleApp::getSampleName() { return "03_DrawingTriangle"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Drawing_a_triangle
 */
void SampleApp::render(double)
{
    EASY_FUNCTION()

    ctx.beginPath();
    ctx.moveTo(75, 50);
    ctx.lineTo(100, 75);
    ctx.lineTo(100, 25);
    ctx.fill();
}
