#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "13_GlobalAlphaExample"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#A_globalAlpha_example
 */
void SampleApp::render(double)
{
    // draw background
    ctx.globalAlpha = 1.0;
    ctx.fillStyle = "#FD0";
    ctx.fillRect(0, 0, 75, 75);
    ctx.fillStyle = "#6C0";
    ctx.fillRect(75, 0, 75, 75);
    ctx.fillStyle = "#09F";
    ctx.fillRect(0, 75, 75, 75);
    ctx.fillStyle = "#F30";
    ctx.fillRect(75, 75, 75, 75);
    ctx.fillStyle = "#FFF";

    // set transparency value
    ctx.globalAlpha = 0.2;

    // Draw semi transparent circles
    for (int i = 0; i < 7; i++) {
        ctx.beginPath();
        ctx.arc(75, 75, 10 + 10 * i, 0, Math.PI * 2, true);
        ctx.fill();
    }
}
