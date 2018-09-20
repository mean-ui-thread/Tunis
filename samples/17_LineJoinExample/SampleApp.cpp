#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "17_LineJoinExample"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#A_lineJoin_example
 */
void SampleApp::render(double)
{
    LineJoin lineJoin[] = {tunis::Round, tunis::bevel, tunis::miter};
    ctx.lineWidth = 10;
    for (int i = 0; i < sizeof(lineJoin)/sizeof(LineJoin); i++) {
      ctx.lineJoin = lineJoin[i];
      ctx.beginPath();
      ctx.moveTo(-5, 5 + i * 40);
      ctx.lineTo(35, 45 + i * 40);
      ctx.lineTo(75, 5 + i * 40);
      ctx.lineTo(115, 45 + i * 40);
      ctx.lineTo(155, 5 + i * 40);
      ctx.stroke();
    }
}
