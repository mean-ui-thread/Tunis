#ifndef TUNISTYPES_H
#define TUNISTYPES_H

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x3.hpp>

namespace tunis
{

using Position = glm::vec2;
using SVGMatrix = glm::mat2x3;
using TCoord = glm::u16vec2;
using Viewport = glm::ivec4;

enum class LineCap : uint8_t
{
    butt = 0, //! The ends of lines are squared off at the endpoints.
    round, //! The ends of lines are rounded.
    square, //! he ends of lines are squared off by adding a box with an equal width and half the height of the line's thickness.
};

enum class LineJoin : uint8_t
{
    bevel = 0, //! Fills an additional triangular area between the common endpoint of connected segments, and the separate outside rectangular corners of each segment.
    round, //! Rounds off the corners of a shape by filling an additional sector of disc centered at the common endpoint of connected segments. The radius for these rounded corners is equal to the line width.
    miter, //! Connected segments are joined by extending their outside edges to connect at a single point, with the effect of filling an additional lozenge-shaped area. This setting is affected by the miterLimit property.
};


enum class FillRule : uint8_t
{
    nonzero, //! The non-zero winding rule, which is the default rule.
    evenodd, //! The even-odd winding rule.
};

enum class CompositeOp : uint8_t
{
    source_over = 0, //! This is the default setting and draws new shapes on top of the existing window content.
    source_in, //! The new shape is drawn only where both the new shape and the destination window overlap. Everything else is made transparent.
    source_out, //! The new shape is drawn where it doesn't overlap the existing window content.
    source_atop, //! The new shape is only drawn where it overlaps the existing window content.
    destination_over, //! New shapes are drawn behind the existing window content.
    destination_in, //! The existing window content is kept where both the new shape and existing window content overlap. Everything else is made transparent.
    destination_out, //! The existing content is kept where it doesn't overlap the new shape.
    destination_atop, //! The existing window is only kept where it overlaps the new shape. The new shape is drawn behind the window content.
    lighter, //! Where both shapes overlap the color is determined by adding color values.
    copy, //! Only the new shape is shown
    Xor, //! Shapes are made transparent where both overlap and drawn normal everywhere else.
    multiply, //! The pixels are of the top layer are multiplied with the corresponding pixel of the bottom layer. A darker picture is the result.
    screen, //! The pixels are inverted, multiplied, and inverted again. A lighter picture is the result (opposite of multiply)
    overlay, //! A combination of multiply and screen. Dark parts on the base layer become darker, and light parts become lighter.
    darken, //! Retains the darkest pixels of both layers.
    lighten, //! Retains the lightest pixels of both layers.
    color_dodge, //! Divides the bottom layer by the inverted top layer.
    color_burn, //! Divides the inverted bottom layer by the top layer, and then inverts the result.
    hard_light, //! A combination of multiply and screen like overlay, but with top and bottom layer swapped.
    soft_light, //! A softer version of hard-light. Pure black or white does not result in pure black or white.
    difference, //! Subtracts the bottom layer from the top layer or the other way round to always get a positive value.
    exclusion, //! Like difference, but with lower contrast.
    hue, //! Preserves the luma and chroma of the bottom layer, while adopting the hue of the top layer.
    saturation, //! Preserves the luma and hue of the bottom layer, while adopting the chroma of the top layer.
    color, //! Preserves the luma of the bottom layer, while adopting the hue and chroma of the top layer.
    luminosity, //! Preserves the hue and chroma of the bottom layer, while adopting the luma of the top layer.
};

enum class TextAlign : uint8_t
{
    left = 0, //! The text is left-aligned.
    right, //! The text is right-aligned.
    center, //! The text is centered.
    start, //! The text is aligned at the normal start of the line (left-aligned for left-to-right locales, right-aligned for right-to-left locales).
    end, //! The text is aligned at the normal end of the line (right-aligned for left-to-right locales, left-aligned for right-to-left locales).
};

enum class TextBaseline : uint8_t
{
    top, //! The text baseline is the top of the em square.
    hanging, //! The text baseline is the hanging baseline. (Used by Tibetan and other Indic scripts.)
    middle, //! The text baseline is the middle of the em square.
    alphabetic, //! The text baseline is the normal alphabetic baseline.
    ideographic, //! The text baseline is the ideographic baseline; this is the bottom of the body of the characters, if the main body of characters protrudes beneath the alphabetic baseline. (Used by Chinese, Japanese and Korean scripts.)
    bottom, //! The text baseline is the bottom of the bounding box. This differs from the ideographic baseline in that the ideographic baseline doesn't consider descenders.
};

enum class Direction : uint8_t
{
    ltr, //! The text direction is left-to-right.
    rtl, //! The text direction is right-to-left.
    inherit //! The text direction is inherited as appropriate.
};

}

#endif // TUNISTYPES_H
