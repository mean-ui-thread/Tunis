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
#ifndef TUNISCONTEXTSTATE_H
#define TUNISCONTEXTSTATE_H

#include <TunisPaint.h>
#include <TunisFontDef.h>
#include <TunisPath2D.h>

namespace tunis
{
    namespace detail
    {
        class ContextPriv;
    }

class ContextState
{
public:

    friend detail::ContextPriv;

    /*!
     * \brief currentTransform the current transformation matrix.
     */
    SVGMatrix currentTransform = SVGMatrix(1.0f);

    /*!
     * \brief strokeStyle specifies the color or style to use for the lines
     * around shapes. The default is the color black.
     */
    Paint strokeStyle = Black;

    /*!
     * \brief fillStyle specifies the color or style to use inside shapes. The
     * default is the color black.
     */
    Paint fillStyle = Black;

    /*!
     * \brief globalAlpha specifies the alpha value that is applied to shapes
     * and images before they are drawn onto the window. The value is in the
     * range from 0.0 (fully transparent) to 1.0 (fully opaque). The default
     * value is 1.0.
     */
    float globalAlpha = 1.0f;

    /*!
     * \brief lineWidth sets the thickness of lines in space units. When
     * getting, it returns the current value (1.0 by default). When setting,
     * zero, negative, Infinity and NaN values are ignored; otherwise the
     * current value is set to the new value.
     */
    float lineWidth = 1.0f;

    /*!
     * \brief lineCap determines how the end points of every line are drawn.
     * There are three possible values for this property and those are: butt,
     * round and square. By default this property is set to butt.
     */
    LineCap lineCap = LineCap::butt;

    /*!
     * \brief lineJoin determines how two connecting segments (of lines, arcs or
     * curves) with non-zero lengths in a shape are joined together (degenerate
     * segments with zero lengths, whose specified endpoints and control points
     * are exactly at the same position, are skipped). There are three possible
     * values for this property: round, bevel and miter. By default this
     * property is set to miter.
     *
     * \note the lineJoin setting has no effect if the two connected segments
     * have the same direction, because no joining area will be added in this
     * case.
     */
    LineJoin lineJoin = LineJoin::miter;

    /*!
     * \brief miterLimit sets the miter limit ratio in space units. When
     * getting, it returns the current value (10.0 by default). When setting,
     * zero, negative, Infinity and NaN values are ignored; otherwise the
     * current value is set to the new value.
     */
    float miterLimit = 10.0f;

    /*!
     * \brief lineDashOffset  sets the line dash pattern offset or "phase" to
     * achieve a "marching ants" effect, for example.
     */
    float lineDashOffset = 0.0f;

    /*!
     * \brief shadowOffsetX specifies the distance that the shadow will be
     * offset in horizontal distance.
     */
    float shadowOffsetX = 0.0f;

    /*!
     * \brief shadowOffsetY specifies the distance
     * that the shadow will be offset in vertical distance.
     */
    float shadowOffsetY = 0.0f;

    /*!
     * \brief shadowBlur specifies the level of the blurring effect; this value
     * doesn't correspond to a number of pixels and is not affected by the
     * current transformation matrix. The default value is 0.
     */
    float shadowBlur = 0.0f;

    /*!
     * \brief shadowColor specifies the color of the shadow. The default value
     * is fully-transparent black.
     */
    Color shadowColor = Transparent;

    /*!
     * \brief globalCompositeOperation sets the type of compositing operation to
     * apply when drawing new shapes, where type is a string identifying which
     * of the compositing or blending mode operations to use. The default is
     * CompositeOpSourceOver
     */
    CompositeOp globalCompositeOperation = CompositeOp::source_over;

    /*!
     * \brief font specifies the current text style being used when drawing
     * text. This string uses the same syntax as the CSS font specifier. The
     * default font is '10px sans-serif'.
     */
    FontDef font = "10px sans-serif";

    /*!
     * \brief textAlign specifies the current text alignment being used when
     * drawing text. Beware that the alignment is based on the x value of the
     * fillText() method. So if textAlign is "center", then the text would be
     * drawn at x - (width / 2). The default value is TextAlignStart.
     */
    TextAlign textAlign = TextAlign::start;

    /*!
     * \brief textBaseline  specifies the current text baseline being used when
     * drawing text. The default value is TextBaselineAlphabetic.
     */
    TextBaseline textBaseline = TextBaseline::alphabetic;

    /*!
     * \brief direction I specifies the current text direction used when drawing
     * text.
     */
    Direction direction = Direction::inherit;

    /*!
     * \brief imageSmoothingEnabled can be set to change if images are smoothed
     * (true, default) or not (false). On getting the imageSmoothingEnabled
     * property, the last value it was set to, is returned.
     *
     * This property is useful for pixel-art themed games, when scaling the
     * window for example. The default resizing algorithm can create blurry
     * effects and ruins the beautiful pixels. Set this property to false in
     * that case.
     */
    bool imageSmoothingEnabled = true;

protected:


    Path2D clipRegion;
    std::vector<float> lineDashes;

};

}

#endif // TUNISCONTEXTSTATE_H
