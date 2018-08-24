#include "SampleApp.h"

#include <stdio.h>

using namespace tunis;
using namespace tunis::color;

const char *SampleApp::getSampleName() { return "04_MovingThePen"; }
int SampleApp::getScreenWidth() { return 320; }
int SampleApp::getScreenHeight() { return 200; }

/*!
 * Based of https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Drawing_a_triangle
 */
void SampleApp::render(int32_t windowWidth, int32_t windowHeight, double frameTime)
{
    ctx.beginFrame(0, 0, windowWidth, windowHeight);

    ctx.beginPath();
    ctx.arc(75, 75, 50, 0, Math.PI * 2, true); // Outer circle
    ctx.moveTo(110, 75);
    ctx.arc(75, 75, 35, 0, Math.PI, false);  // Mouth (clockwise)
    ctx.moveTo(65, 65);
    ctx.arc(60, 65, 5, 0, Math.PI * 2, true);  // Left eye
    ctx.moveTo(95, 65);
    ctx.arc(90, 65, 5, 0, Math.PI * 2, true);  // Right eye
    ctx.stroke();

    ctx.endFrame();
}
