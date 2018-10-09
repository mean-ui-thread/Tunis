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
const char *SampleApp::getSampleName() { return "06_Arcs"; }
int SampleApp::getWindowWidth() { return 320; }
int SampleApp::getWindowHeight() { return 200; }

/*!
 * Based on https://developer.mozilla.org/en-US/docs/Web/API/Canvas_API/Tutorial/Drawing_shapes#Arcs
 */
void SampleApp::render(double)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            ctx.beginPath();
            float x = 25 + j * 50; // x coordinate
            float y = 25 + i * 50; // y coordinate
            float radius = 20; // Arc radius
            float startAngle = 0; // Starting point on circle
            float endAngle = Math.PI + (Math.PI * j) / 2; // End point on circle
            bool anticlockwise = i % 2 != 0; // clockwise or anticlockwise

            ctx.arc(x, y, radius, startAngle, endAngle, anticlockwise);

            if (i > 1) {
                ctx.fill();
            } else {
                ctx.stroke();
            }
        }
    }
}
