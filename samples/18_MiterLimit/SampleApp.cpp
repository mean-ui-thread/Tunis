#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "18_MiterLimit"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#A_demo_of_the_miterLimit_property
 */
void SampleApp::render(double t)
{
    // Draw guides
    ctx.strokeStyle = "#09f";
    ctx.lineWidth   = 2;
    ctx.strokeRect(-5, 50, 325, 50);

    // Set line styles
    ctx.strokeStyle = "#000";
    ctx.lineWidth = 10;

    ctx.miterLimit = ((Math.sin(t) + 1.0f) / 2.0f) * 20.0f; // 0..20

    // Draw lines
    ctx.beginPath();
    ctx.moveTo(0, 100);
    for (int i = 0; i < 24 ; i++)
    {
        float dy = i % 2 == 0 ? 25 : -25;
        ctx.lineTo(Math.pow(i, 1.5) * 2, 75 + dy);
    }
    ctx.stroke();
}
