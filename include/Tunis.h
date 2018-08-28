#ifndef TUNIS_H
#define TUNIS_H

#include <memory>

#include "TunisColor.h"
#include "TunisPaint.h"
#include "TunisPath2D.h"
#include "TunisTexture.h"
#include "TunisMath.h"

#include <glm/mat2x3.hpp>

namespace tunis
{

class Backend;

class Context
{
public:

    enum FillRule {
        nonzero, //!< The non-zero winding rule, which is the default rule.
        evenodd, //!< The even-odd winding rule.
    };


    Context();
    ~Context();

    Context(const Context &) = delete;
    Context &operator=(const Context &) = delete;

    void setBackgroundColor(const Color &color);

    void beginFrame(int32_t x, int32_t y, int32_t w, int32_t h);
    void endFrame();



    Paint fillStyle = color::Black;
    Paint strokeStyle = color::Black;
    float lineWidth = 1.0f;

    /*!
     * \brief fillRect draws a filled rectangle whose starting point is at the
     * coordinates (x, y) with the specified width and height and whose style is
     * determined by the fillStyle attribute.
     *
     * \param x The x component of the coordinates for the rectangle's starting
     * point.
     * \param y The y component of the coordinates for the rectangle's starting
     * point.
     * \param width The rectangle's width.
     * \param height The rectangle's height.
     */
    void fillRect(float x, float y, float width, float height);

    /*!
     * \brief strokeRect paints a rectangle which has a starting point at (x, y)
     * and has a w width and an h height onto the canvas, using the current
     * stroke style.
     *
     * \param x The x axis of the coordinate for the rectangle starting point.
     * \param y The y axis of the coordinate for the rectangle starting point.
     * \param width The rectangle's width.
     * \param height The rectangle's height.
     */
    void strokeRect(float x, float y, float width, float height);

    /*!
     * \brief clearRect sets all pixels in the rectangle defined by starting
     * point (x, y) and size (width, height) to transparent black, erasing any
     * previously drawn content.
     *
     * \param x The x axis of the coordinate for the rectangle starting point.
     * \param y The y axis of the coordinate for the rectangle starting point.
     * \param width The rectangle's width.
     * \param height The rectangle's height.
     */
    void clearRect(float x, float y, float width, float height);


    /*!
     * \brief beginPath starts a new path by emptying the list of sub-paths.
     * Call this method when you want to create a new path.
     */
    void beginPath();

    /*!
     * \brief closePath causes the point of the pen to move back to the start of
     * the current sub-path. It tries to add a straight line (but does not
     * actually draw it) from the current point to the start.
     *
     * \note If the shape has already been closed or has only one point, this
     * function does nothing.
     */
    void closePath();

    /*!
     * \brief moveTo moves the starting point of a new sub-path to the (x, y)
     * coordinates.
     *
     * \param x The x axis of the point.
     * \param y The y axis of the point.
     */
    void moveTo(float x, float y);

    /*!
     * \brief lineTo connects the last point in the sub-path to the x, y
     * coordinates with a straight line (but does not actually draw it).
     *
     * \param x The x axis of the coordinate for the end of the line.
     * \param y The y axis of the coordinate for the end of the line.
     */
    void lineTo(float x, float y);

    /*!
     * \brief bezierCurveTo adds a cubic Bézier curve to the path. It requires
     * three points. The first two points are control points and the third one
     * is the end point. The starting point is the last point in the current
     * path, which can be changed using moveTo() before creating the Bézier
     * curve.
     *
     * \param cp1x The x coordinate of the first control point.
     * \param cp1y The y coordinate of the first control point.
     * \param cp2x The x coordinate of the second control point.
     * \param cp2y The y coordinate of the second control point.
     * \param x The x coordinate of the end point.
     * \param y The y coordinate of the end point.
     */
    void bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x,
                       float y);

    /*!
     * \brief quadraticCurveTo adds a quadratic Bézier curve to the path. It
     * requires two points. The first point is a control point and the second
     * one is the end point. The starting point is the last point in the current
     * path, which can be changed using moveTo() before creating the quadratic
     * Bézier curve.
     *
     * \param cpx The x axis of the coordinate for the control point.
     * \param cpy The y axis of the coordinate for the control point.
     * \param x The x axis of the coordinate for the end point.
     * \param y The y axis of the coordinate for the end point.
     */
    void quadraticCurveTo(float cpx, float cpy, float x, float y);

    /*!
     * \brief arc adds an arc to the path which is centered at (x, y) position
     * with radius r starting at startAngle and ending at endAngle going in the
     * given direction by anticlockwise (defaulting to clockwise).
     *
     * \param x The x coordinate of the arc's center.
     * \param y The y coordinate of the arc's center.
     * \param radius The arc's radius.
     * \param startAngle The angle at which the arc starts, measured clockwise
     * from the positive x axis and expressed in radians.
     * \param endAngle The angle at which the arc ends, measured clockwise from
     * the positive x axis and expressed in radians.
     * \param anticlockwise An optional boolean which, if true, causes the arc
     * to be drawn counter-clockwise between the two angles. By default it is
     * drawn clockwise.
     */
    void arc(float x, float y, float radius, float startAngle, float endAngle,
             bool anticlockwise = false);

    /*!
     * \brief arcTo adds an arc to the path with the given control points and
     * radius.
     *
     * The arc drawn will be a part of a circle, never elliptical. Typical use
     * could be making a rounded corner.
     *
     * One way to think about the arc drawn is to imagine two straight segments,
     * from the starting point (latest point in current path) to the first
     * control point, and then from the first control point to the second
     * control point. These two segments form a sharp corner with the first
     * control point being in the corner. Using arcTo, the corner will instead
     * be an arc with the given radius.
     *
     * The arc is tangential to both segments, which can sometimes produce
     * surprising results, e.g. if the radius given is larger than the distance
     * between the starting point and the first control point.
     *
     * If the radius specified doesn't make the arc meet the starting point
     * (latest point in the current path), the starting point is connected to
     * the arc with a straight line segment.
     *
     * \param x1 x-axis coordinates for the first control point.
     * \param y1 y-axis coordinates for the first control point.
     * \param x2 x-axis coordinates for the second control point.
     * \param y2 y-axis coordinates for the second control point.
     * \param radius The arc's radius.
     */
    void arcTo(float x1, float y1, float x2, float y2, float radius);

    /*!
     * \brief ellipse adds an ellipse to the path which is centered at (x, y)
     * position with the radii radiusX and radiusY starting at startAngle and
     * ending at endAngle going in the given direction by anticlockwise
     * (defaulting to clockwise).
     *
     * \param x The x axis of the coordinate for the ellipse's center.
     * \param y The y axis of the coordinate for the ellipse's center.
     * \param radiusX The ellipse's major-axis radius.
     * \param radiusY The ellipse's minor-axis radius.
     * \param rotation The rotation for this ellipse, expressed in radians.
     * \param startAngle The starting point, measured from the x axis, from
     * which it will be drawn, expressed in radians.
     * \param endAngle The end ellipse's angle to which it will be drawn,
     * expressed in radians.
     * \param anticlockwise An optional boolean which, if true, draws the
     * ellipse anticlockwise (counter-clockwise), otherwise in a clockwise
     * direction.
     */
    void ellipse(float x, float y, float radiusX, float radiusY, float rotation,
                 float startAngle, float endAngle, bool anticlockwise = false);

    /*!
     * \brief rect creates a path for a rectangle at position (x, y) with a size
     * that is determined by width and height. Those four points are connected
     * by straight lines and the sub-path is marked as closed, so that you can
     * fill or stroke this rectangle.
     * \param x The x coordinate for the left side of the rectangle.
     * \param y The y coordinate for the top of the rectangle.
     * \param width The rectangle's width.
     * \param height The rectangle's height.
     */
    void rect(float x, float y, float width, float height);

    /*!
     * \brief fill fills the current path with the current fill style using the
     * non-zero or even-odd winding rule.
     *
     * \param fillRule The algorithm by which to determine if a point is inside
     * a path or outside a path.
     */
    void fill(FillRule fillRule = nonzero);

    /*!
     * \brief fill fills the given path with the current fill style using the
     * non-zero or even-odd winding rule.
     *
     * \param path A Path2D path to fill.
     * \param fillRule The algorithm by which to determine if a point is inside
     * a path or outside a path.
     */
    void fill(Path2D &path, FillRule fillRule = nonzero);

    /*!
     * \brief stroke strokes the current or given path with the current stroke
     * style using the non-zero winding rule.
     */
    void stroke();

    /*!
     * \brief stroke strokes the given path with the current stroke style using
     * the non-zero winding rule.
     *
     * \param path A Path2D path to stroke.
     */
    void stroke(Path2D &path);

private:

    void pushColorRect(float x, float y, float width, float height,
                       const Color &color);

    Path2D m_currentPath;

    enum RenderType
    {
        RenderDefault2D = 0,
    };

    enum
    {
        _renderType = 0,
        _texture,
        _vertexStartOffset,
        _vertexCount
    };

    SoA<
        RenderType, // _renderType
        Texture,    // _texture
        size_t,     // _vertexStartOffset
        size_t      // _vertexCount
    > m_batches;

    SVGMatrix m_xform;

    std::unique_ptr<Backend> m_pBackend;
    std::vector<Texture> m_textures;

};

}

#include <Tunis.inl>

#endif // TUNIS_H
