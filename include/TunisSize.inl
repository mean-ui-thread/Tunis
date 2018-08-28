#ifndef TUNISSIZE_INL
#define TUNISSIZE_INL

#include <TunisSize.h>

#include <glm/gtc/epsilon.hpp>

namespace tunis
{

constexpr inline Size::Size() noexcept
    : m_width(-1.), m_height(-1.)
{

}

constexpr inline Size::Size(const Size &sz) noexcept
    : m_width(sz.m_width), m_height(sz.m_height)
{

}

constexpr inline Size::Size(float w, float h) noexcept
    : m_width(w), m_height(h)
{

}

inline bool Size::isNull() const noexcept
{
    return glm::epsilonEqual(m_width, 0.0f, glm::epsilon<float>()) &&
            glm::epsilonEqual(m_height, 0.0f, glm::epsilon<float>());
}

constexpr inline bool Size::isEmpty() const noexcept
{
    return m_width <= .0f || m_height <= .0f;
}

constexpr inline bool Size::isValid() const noexcept
{
    return m_width >= .0f && m_height >= .0f;
}

constexpr inline float &Size::width() noexcept
{
    return m_width;
}

constexpr inline float &Size::height() noexcept
{
    return m_height;
}

constexpr inline float Size::width() const noexcept
{
    return m_width;
}

constexpr inline float Size::height() const noexcept
{
    return m_height;
}

constexpr inline void Size::setWidth(float w) noexcept
{
    m_width = w;
}

constexpr inline void Size::setHeight(float h) noexcept
{
    m_height = h;
}

constexpr inline Size Size::transposed() const noexcept
{
    return Size(m_height, m_width);
}

constexpr inline void Size::scale(float w, float h, AspectRatioMode mode) noexcept
{
    scale(Size(w, h), mode);
}

constexpr inline void Size::scale(const Size &s, AspectRatioMode mode) noexcept
{
    *this = scaled(s, mode);
}

constexpr inline Size Size::scaled(float w, float h, AspectRatioMode mode) const noexcept
{
    return scaled(Size(w, h), mode);
}

constexpr inline Size Size::scaled(const Size &s, AspectRatioMode mode) const noexcept
{
    return scaled(s, mode);
}

constexpr inline Size &Size::operator+=(const Size &s) noexcept
{
    m_width += s.m_width; m_height += s.m_height; return *this;
}

constexpr inline Size &Size::operator-=(const Size &s) noexcept
{
    m_width -= s.m_width; m_height -= s.m_height; return *this;
}

constexpr inline Size &Size::operator*=(float c) noexcept
{
    m_width *= c; m_height *= c; return *this;
}

constexpr inline const Size Size::operator+(const Size &s) noexcept
{
    return Size(m_width + s.m_width, m_height + s.m_height);
}

constexpr inline const Size Size::operator-(const Size &s) noexcept
{
    return Size(m_width - s.m_width, m_height - s.m_height);
}

constexpr inline const Size Size::operator*(float c) noexcept
{
    return Size(m_width * c, m_height * c);
}

constexpr inline const Size Size::operator*(const Size &s) noexcept
{
    return Size(m_width * s.m_width, m_height * s.m_height);
}

constexpr inline Size &Size::operator/=(float c)
{
    assert(c > .0f && c < .0f);
    m_width = m_width/c; m_height = m_height/c;
    return *this;
}
constexpr inline const Size Size::operator/(float c)
{
    assert(c > .0f && c < .0f);
    return Size(m_width / c, m_height / c);
}

constexpr inline Size Size::expandedTo(const Size &s) const noexcept
{
    return Size(glm::max(m_width, s.m_width), glm::max(m_height, s.m_height));
}

constexpr inline Size Size::boundedTo(const Size &s) const noexcept
{
    return Size(glm::min(m_width, s.m_width), glm::min(m_height, s.m_height));
}


}

inline bool operator==(const tunis::Size &s1, const tunis::Size &s2) noexcept
{
    return glm::epsilonEqual(s1.width(), s2.width(), glm::epsilon<float>()) &&
            glm::epsilonEqual(s1.height(), s2.height(), glm::epsilon<float>());
}

inline bool operator!=(const tunis::Size &s1, const tunis::Size &s2) noexcept
{
    return glm::epsilonNotEqual(s1.width(), s2.width(), glm::epsilon<float>()) ||
            glm::epsilonNotEqual(s1.height(), s2.height(), glm::epsilon<float>());
}

#endif // TUNISSIZE_INL
