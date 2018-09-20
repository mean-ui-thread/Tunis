#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "14_RGBAExample"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#An_example_using_rgba()
 */
void SampleApp::render(double)
{
    // Draw background
    ctx.fillStyle = rgb(255, 221, 0);
    ctx.fillRect(0, 0, 150, 37.5);
    ctx.fillStyle = rgb(102, 204, 0);
    ctx.fillRect(0, 37.5, 150, 37.5);
    ctx.fillStyle = rgb(0, 153, 255);
    ctx.fillRect(0, 75, 150, 37.5);
    ctx.fillStyle = rgb(255, 51, 0);
    ctx.fillRect(0, 112.5, 150, 37.5);

    // Draw semi transparent rectangles
    for (int i = 0; i < 10; i++) {
        ctx.fillStyle = rgba(255, 255, 255, (i + 1) / 10.0f);
        for (int j = 0; j < 4; j++) {
            ctx.fillRect(5 + i * 14, 5 + j * 37.5, 14, 27.5);
        }
    }
}
