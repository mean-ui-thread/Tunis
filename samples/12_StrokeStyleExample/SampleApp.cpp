#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "12_StrokeStyleExample"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#A_strokeStyle_example
 */
void SampleApp::render(double)
{
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 6; j++) {
        ctx.strokeStyle = rgb(0, Math.floor(255 - 42.5 * i),
                              Math.floor(255 - 42.5 * j));
        ctx.beginPath();
        ctx.arc(12.5 + j * 25, 12.5 + i * 25, 10, 0, Math.PI * 2, true);
        ctx.stroke();
      }
    }
}
