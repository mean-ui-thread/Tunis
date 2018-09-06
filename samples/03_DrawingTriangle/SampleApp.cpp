#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "03_DrawingTriangle"; }
int SampleApp::getWindowWidth() { return 500; }
int SampleApp::getWindowHeight() { return 500; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Drawing_a_triangle
 */
void SampleApp::render(double)
{
//    ctx.beginPath();
//    ctx.moveTo(75, 50);
//    ctx.lineTo(100, 75);
//    ctx.lineTo(100, 25);
//    ctx.fill();


    ctx.beginPath();
    ctx.moveTo(100,100);
    ctx.lineTo(200,100);
    ctx.lineTo(250,0);
    ctx.lineTo(300,100);
    ctx.lineTo(400,100);
    ctx.lineTo(325,175);
    ctx.lineTo(350,275);
    ctx.lineTo(250,200);
    ctx.lineTo(150,275);
    ctx.lineTo(175,175);
    ctx.fill();
}
