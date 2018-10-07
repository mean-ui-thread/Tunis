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

const char *SampleApp::getSampleName() { return "21_CreateRadialGradient"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Applying_styles_and_colors#A_createRadialGradient_example
 */
void SampleApp::render(double)
{
    // Create gradients
    auto radgrad = ctx.createRadialGradient(45, 45, 10, 52, 50, 30);
    radgrad.addColorStop(0, "#A7D30C");
    radgrad.addColorStop(0.9, "#019F62");
    radgrad.addColorStop(1, "rgba(1, 159, 98, 0)");

    auto radgrad2 = ctx.createRadialGradient(105, 105, 20, 112, 120, 50);
    radgrad2.addColorStop(0, "#FF5F98");
    radgrad2.addColorStop(0.75, "#FF0188");
    radgrad2.addColorStop(1, "rgba(255, 1, 136, 0)");

    auto radgrad3 = ctx.createRadialGradient(95, 15, 15, 102, 20, 40);
    radgrad3.addColorStop(0, "#00C9FF");
    radgrad3.addColorStop(0.8, "#00B5E2");
    radgrad3.addColorStop(1, "rgba(0, 201, 255, 0)");

    auto radgrad4 = ctx.createRadialGradient(0, 150, 50, 0, 140, 90);
    radgrad4.addColorStop(0, "#F4F201");
    radgrad4.addColorStop(0.8, "#E4C700");
    radgrad4.addColorStop(1, "rgba(228, 199, 0, 0)");

    // draw shapes
    ctx.fillStyle = radgrad4;
    ctx.fillRect(0, 0, 150, 150);
    ctx.fillStyle = radgrad3;
    ctx.fillRect(0, 0, 150, 150);
    ctx.fillStyle = radgrad2;
    ctx.fillRect(0, 0, 150, 150);
    ctx.fillStyle = radgrad;
    ctx.fillRect(0, 0, 150, 150);
}
