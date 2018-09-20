#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "16_LineCapExample"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#A_lineCap_example
 */
void SampleApp::render(double)
{
    LineCap lineCap[] = {tunis::butt, tunis::round, tunis::square};

    // Draw guides
    ctx.lineWidth = 1;
    ctx.strokeStyle = "#09f";
    ctx.beginPath();
    ctx.moveTo(10, 10);
    ctx.lineTo(140, 10);
    ctx.moveTo(10, 140);
    ctx.lineTo(140, 140);
    ctx.stroke();

    // Draw lines
    ctx.strokeStyle = Black;
    for (auto i = 0; i < sizeof(lineCap)/sizeof(LineCap); i++) {
      ctx.lineWidth = 15;
      ctx.lineCap = lineCap[i];
      ctx.beginPath();
      ctx.moveTo(25 + i * 50, 10);
      ctx.lineTo(25 + i * 50, 140);
      ctx.stroke();
    }
}
