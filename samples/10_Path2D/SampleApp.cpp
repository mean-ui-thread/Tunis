#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "10_Path2D"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Path2D_example
 */
void SampleApp::render(double)
{
    Path2D rectangle;
    rectangle.rect(10, 10, 50, 50);

    Path2D circle;
    circle.moveTo(125, 35);
    circle.arc(100, 35, 25, 0, 2 * Math.PI);

    ctx.stroke(rectangle);
    ctx.fill(circle);
}
