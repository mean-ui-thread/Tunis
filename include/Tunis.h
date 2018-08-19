#ifndef TUNIS_H
#define TUNIS_H

#include <memory>
#include "TunisPaint.h"

namespace tunis
{

class Backend;

class Context
{
public:
    Context();
    ~Context();

    void setBackgroundColor(const glm::vec4 &color);

    void beginFrame(int32_t x, int32_t y, int32_t w, int32_t h);
    void endFrame();

    Paint fillStyle;

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


private:
    std::unique_ptr<Backend> m_pBackend;
};

}

#endif // TUNIS_H
