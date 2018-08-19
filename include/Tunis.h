#ifndef TUNIS_H
#define TUNIS_H

#include <memory>

#include "TunisColor.h"
#include "TunisPaint.h"
#include "TunisTexture.h"

namespace tunis
{

class Backend;

class Context
{
public:
    Context();
    ~Context();

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
     * \param x The x axis of the coordinate for the rectangle starting point.
     * \param y The y axis of the coordinate for the rectangle starting point.
     * \param width The rectangle's width.
     * \param height The rectangle's height.
     */
    void clearRect(float x, float y, float width, float height);

private:

    void pushColorRect(float x, float y, float width, float height, const Color &color);

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

    std::unique_ptr<Backend> m_pBackend;
    std::vector<Texture> m_textures;

};

}

#endif // TUNIS_H
