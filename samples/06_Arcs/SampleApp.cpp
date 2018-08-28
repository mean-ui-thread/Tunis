#include "SampleApp.h"

#include <stdio.h>

#include <easy/profiler.h>

using namespace tunis;
using namespace tunis::color;

const char *SampleApp::getSampleName() { return "06_Arcs"; }
int SampleApp::getScreenWidth() { return 320; }
int SampleApp::getScreenHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Arcs
 */
void SampleApp::render(int32_t windowWidth, int32_t windowHeight, double frameTime)
{
    EASY_FUNCTION()

    ctx.beginFrame(0, 0, windowWidth, windowHeight);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            ctx.beginPath();
            float x = 25 + j * 50; // x coordinate
            float y = 25 + i * 50; // y coordinate
            float radius = 20; // Arc radius
            float startAngle = 0; // Starting point on circle
            float endAngle = Math.PI + (Math.PI * j) / 2; // End point on circle
            bool anticlockwise = i % 2 != 0; // clockwise or anticlockwise

            ctx.arc(x, y, radius, startAngle, endAngle, anticlockwise);

            if (i > 1) {
                ctx.fill();
            } else {
                ctx.stroke();
            }
        }
    }

    ctx.endFrame();
}
