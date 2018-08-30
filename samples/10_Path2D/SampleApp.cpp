#include "SampleApp.h"

#include <stdio.h>

#include <easy/profiler.h>

using namespace tunis;
using namespace tunis::color;

const char *SampleApp::getSampleName() { return "10_Path2D"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Arcs
 */
void SampleApp::render(double)
{
    EASY_FUNCTION()

    Path2D rectangle;
    rectangle.rect(10, 10, 50, 50);

    Path2D circle;
    circle.moveTo(125, 35);
    circle.arc(100, 35, 25, 0, 2 * Math.PI);

    ctx.stroke(rectangle);
    ctx.fill(circle);
}
