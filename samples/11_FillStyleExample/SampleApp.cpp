#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "11_FillStyleExample"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#A_fillStyle_example
 */
void SampleApp::render(double)
{
    for (int i = 0; i < 6; i++) {
      for (int j = 0; j < 6; j++) {
        ctx.fillStyle = rgb(Math.floor(255 - 42.5 * i),
                            Math.floor(255 - 42.5 * j), 0);
        ctx.fillRect(j * 25, i * 25, 25, 25);
      }
    }
}
