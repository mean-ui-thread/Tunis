#ifndef TUNISRECT_H
#define TUNISRECT_H

#include <TunisTypes.h>
#include <TunisSize.h>

namespace tunis
{

class Rect
{
public:
    constexpr Rect() noexcept;
    constexpr Rect(const Point &topleft, const Size &size) noexcept;
    constexpr Rect(const Point &topleft, const Point &bottomRight) noexcept;
    constexpr Rect(float left, float top, float width, float height) noexcept;
    constexpr Rect(const Rect &rect) noexcept;
    bool isNull() const noexcept;
    constexpr bool isEmpty() const noexcept;
    constexpr bool isValid() const noexcept;
    constexpr Rect normalized() const noexcept;
    constexpr float left() const noexcept;
    constexpr float top() const noexcept;
    constexpr float right() const noexcept;
    constexpr float bottom() const noexcept;
    constexpr float &x() noexcept;
    constexpr float x() const noexcept;
    constexpr float &y() noexcept;
    constexpr float y() const noexcept;
    constexpr void setLeft(float pos) noexcept;
    constexpr void setTop(float pos) noexcept;
    constexpr void setRight(float pos) noexcept;
    constexpr void setBottom(float pos) noexcept;
    constexpr void setX(float pos) noexcept { setLeft(pos); }
    constexpr void setY(float pos) noexcept { setTop(pos); }
    constexpr Point topLeft() const noexcept { return Point(m_x, m_y); }
    constexpr Point bottomRight() const noexcept { return Point(m_x+m_width, m_y+m_height); }
    constexpr Point topRight() const noexcept { return Point(m_x+m_width, m_y); }
    constexpr Point bottomLeft() const noexcept { return Point(m_x, m_y+m_height); }
    constexpr Point center() const noexcept;
    constexpr void setTopLeft(const Point &p) noexcept;
    constexpr void setBottomRight(const Point &p) noexcept;
    constexpr void setTopRight(const Point &p) noexcept;
    constexpr void setBottomLeft(const Point &p) noexcept;
    constexpr void moveLeft(float pos) noexcept;
    constexpr void moveTop(float pos) noexcept;
    constexpr void moveRight(float pos) noexcept;
    constexpr void moveBottom(float pos) noexcept;
    constexpr void moveTopLeft(const Point &p) noexcept;
    constexpr void moveBottomRight(const Point &p) noexcept;
    constexpr void moveTopRight(const Point &p) noexcept;
    constexpr void moveBottomLeft(const Point &p) noexcept;
    constexpr void moveCenter(const Point &p) noexcept;
    constexpr void translate(float dx, float dy) noexcept;
    constexpr void translate(const Point &p) noexcept;
    constexpr Rect translated(float dx, float dy) const noexcept;
    constexpr Rect translated(const Point &p) const noexcept;
    constexpr Rect transposed() const noexcept;
    constexpr void moveTo(float x, float y) noexcept;
    constexpr void moveTo(const Point &p) noexcept;
    constexpr void setRect(float x, float y, float m_width, float m_height) noexcept;
    constexpr void getRect(float *x, float *y, float *m_width, float *m_height) const noexcept;
    constexpr void setCoords(float x1, float y1, float x2, float y2) noexcept;
    constexpr void getCoords(float *x1, float *y1, float *x2, float *y2) const noexcept;
    constexpr void adjust(float x1, float y1, float x2, float y2) noexcept;
    constexpr Rect adjusted(float x1, float y1, float x2, float y2) const noexcept;
    constexpr Size size() const noexcept;
    constexpr float width() const noexcept;
    constexpr float height() const noexcept;
    constexpr void setWidth(float m_width) noexcept;
    constexpr void setHeight(float m_height) noexcept;
    constexpr void setSize(const Size &s) noexcept;
    constexpr Rect operator|(const Rect &r) const noexcept;
    constexpr Rect operator&(const Rect &r) const noexcept;
    constexpr Rect& operator|=(const Rect &r) noexcept;
    constexpr Rect& operator&=(const Rect &r) noexcept;
    constexpr bool contains(const Rect &r) const noexcept;
    constexpr bool contains(const Point &p) const noexcept;
    constexpr bool contains(float x, float y) const noexcept;
    constexpr Rect united(const Rect &other) const noexcept;
    constexpr Rect intersected(const Rect &other) const noexcept;
    constexpr bool intersects(const Rect &r) const noexcept;
private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
};

}


#include <TunisRect.inl>


#endif // TUNISRECT_H
