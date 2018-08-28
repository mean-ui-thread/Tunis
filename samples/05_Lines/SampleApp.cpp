#include "SampleApp.h"

#include <stdio.h>

#include <easy/profiler.h>

using namespace tunis;
using namespace tunis::color;

const char *SampleApp::getSampleName() { return "05_Lines"; }
int SampleApp::getScreenWidth() { return 320; }
int SampleApp::getScreenHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Lines
 */
void SampleApp::render(int32_t windowWidth, int32_t windowHeight, double frameTime)
{
    EASY_FUNCTION()

    ctx.beginFrame(0, 0, windowWidth, windowHeight);

    // Filled triangle
    ctx.beginPath();
    ctx.moveTo(25, 25);
    ctx.lineTo(105, 25);
    ctx.lineTo(25, 105);
    ctx.fill();

    // Stroked triangle
    ctx.beginPath();
    ctx.moveTo(125, 125);
    ctx.lineTo(125, 45);
    ctx.lineTo(45, 125);
    ctx.closePath();
    ctx.stroke();

    ctx.endFrame();
}
