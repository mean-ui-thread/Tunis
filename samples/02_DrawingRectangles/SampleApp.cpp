#include "SampleApp.h"

#include <stdio.h>

const char *SampleApp::getSampleName() { return "02_DrawingRectangles"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Rectangular_shape_example
 */
void SampleApp::render(double)
{
    ctx.fillRect(25, 25, 100, 100);
    ctx.clearRect(45, 45, 60, 60);
    ctx.strokeRect(50, 50, 50, 50);
}
