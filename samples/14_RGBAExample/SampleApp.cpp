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

std::unique_ptr<SampleApp> SampleApp::create() { return std::make_unique<SampleApp>(); }
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
