/**
 * MIT License
 *
 * Copyright (c) 2018 Matt Chiasson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/
#include "SampleApp.h"

std::unique_ptr<SampleApp> SampleApp::create() { return std::unique_ptr<SampleApp>(new SampleApp()); }
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
