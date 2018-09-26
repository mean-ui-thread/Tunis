#include "SampleApp.h"

const char *SampleApp::getSampleName() { return "19_UsingLineDashes"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

static float offset = 0.0f;

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#Using_line_dashes
 */
void SampleApp::render(double)
{

    ctx.setLineDash({4, 2});
    ctx.lineDashOffset = -offset;
    ctx.strokeRect(10, 10, 100, 100);

    offset+=1.0f;
    if (offset > 16) {
        offset = 0;
    }
}
