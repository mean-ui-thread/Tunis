#ifndef TUNISTYPES_H
#define TUNISTYPES_H

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x3.hpp>

#include <vector>

namespace tunis
{

using Position = glm::vec2;
using SVGMatrix = glm::mat2x3;
using TCoord = glm::u16vec2;
using Viewport = glm::ivec4;

enum LineCap
{
    LineCapButt = 0, //! The ends of lines are squared off at the endpoints.
    LineCapRound, //! The ends of lines are rounded.
    LineCapSquare, //! he ends of lines are squared off by adding a box with an equal width and half the height of the line's thickness.
};

enum LineJoin
{
    LineJoinBevel = 0, //! Fills an additional triangular area between the common endpoint of connected segments, and the separate outside rectangular corners of each segment.
    LineJoinRound, //! Rounds off the corners of a shape by filling an additional sector of disc centered at the common endpoint of connected segments. The radius for these rounded corners is equal to the line width.
    LineJoinMiter, //! Connected segments are joined by extending their outside edges to connect at a single point, with the effect of filling an additional lozenge-shaped area. This setting is affected by the miterLimit property.
};


enum Fill
{
    FillNonZero, //! The non-zero winding rule, which is the default rule.
    FillEvenOdd, //! The even-odd winding rule.
};

enum CompositeOp
{
    CompositeOpSourceOver = 0, //! This is the default setting and draws new shapes on top of the existing window content.
    CompositeOpSourceIn, //! The new shape is drawn only where both the new shape and the destination window overlap. Everything else is made transparent.
    CompositeOpSourceOut, //! The new shape is drawn where it doesn't overlap the existing window content.
    CompositeOpSourceAtop, //! The new shape is only drawn where it overlaps the existing window content.
    CompositeOpDestinationOver, //! New shapes are drawn behind the existing window content.
    CompositeOpDestinationIn, //! The existing window content is kept where both the new shape and existing window content overlap. Everything else is made transparent.
    CompositeOpDestinationOut, //! The existing content is kept where it doesn't overlap the new shape.
    CompositeOpDestinationAtop, //! The existing window is only kept where it overlaps the new shape. The new shape is drawn behind the window content.
    CompositeOpLighter, //! Where both shapes overlap the color is determined by adding color values.
    CompositeOpCopy, //! Only the new shape is shown
    CompositeOpXor, //! Shapes are made transparent where both overlap and drawn normal everywhere else.
    CompositeOpMultiply, //! The pixels are of the top layer are multiplied with the corresponding pixel of the bottom layer. A darker picture is the result.
    CompositeOpScreen, //! The pixels are inverted, multiplied, and inverted again. A lighter picture is the result (opposite of multiply)
    CompositeOpOverlay, //! A combination of multiply and screen. Dark parts on the base layer become darker, and light parts become lighter.
    CompositeOpDarken, //! Retains the darkest pixels of both layers.
    CompositeOpLighten, //! Retains the lightest pixels of both layers.
    CompositeOpColorDodge, //! Divides the bottom layer by the inverted top layer.
    CompositeOpColorBurn, //! Divides the inverted bottom layer by the top layer, and then inverts the result.
    CompositeOpHardLight, //! A combination of multiply and screen like overlay, but with top and bottom layer swapped.
    CompositeOpSoftLight, //! A softer version of hard-light. Pure black or white does not result in pure black or white.
    CompositeOpDifference, //! Subtracts the bottom layer from the top layer or the other way round to always get a positive value.
    CompositeOpExclusion, //! Like difference, but with lower contrast.
    CompositeOpHue, //! Preserves the luma and chroma of the bottom layer, while adopting the hue of the top layer.
    CompositeOpSaturation, //! Preserves the luma and hue of the bottom layer, while adopting the chroma of the top layer.
    CompositeOpColor, //! Preserves the luma of the bottom layer, while adopting the hue and chroma of the top layer.
    CompositeOpLuminosity, //! Preserves the hue and chroma of the bottom layer, while adopting the luma of the top layer.
};

enum TextAlign
{
    TextAlignLeft = 0, //! The text is left-aligned.
    TextAlignRight, //! The text is right-aligned.
    TextAlignCenter, //! The text is centered.
    TextAlignStart, //! The text is aligned at the normal start of the line (left-aligned for left-to-right locales, right-aligned for right-to-left locales).
    TextAlignEnd, //! The text is aligned at the normal end of the line (right-aligned for left-to-right locales, left-aligned for right-to-left locales).
};

enum TextBaseline
{
    TextBaselineTop, //! The text baseline is the top of the em square.
    TextBaselineHanging, //! The text baseline is the hanging baseline. (Used by Tibetan and other Indic scripts.)
    TextBaselineMiddle, //! The text baseline is the middle of the em square.
    TextBaselineAlphabetic, //! The text baseline is the normal alphabetic baseline.
    TextBaselineIdeographic, //! The text baseline is the ideographic baseline; this is the bottom of the body of the characters, if the main body of characters protrudes beneath the alphabetic baseline. (Used by Chinese, Japanese and Korean scripts.)
    TextBaselineBottom, //! The text baseline is the bottom of the bounding box. This differs from the ideographic baseline in that the ideographic baseline doesn't consider descenders.
};

enum Direction
{
    DirectionLtr, //! The text direction is left-to-right.
    DirectionRtl, //! The text direction is right-to-left.
    DirectionInherit //! The text direction is inherited as appropriate.
};

}

#endif // TUNISTYPES_H
