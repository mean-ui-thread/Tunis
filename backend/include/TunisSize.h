#ifndef TUNISSIZE_H
#define TUNISSIZE_H

namespace tunis
{

enum AspectRatioMode {
    IgnoreAspectRatio,
    KeepAspectRatio,
    KeepAspectRatioByExpanding
};

class Size
{
public:
    constexpr Size() noexcept;
    constexpr Size(const Size &sz) noexcept;
    constexpr Size(float w, float h) noexcept;
    inline bool isNull() const noexcept;
    constexpr inline bool isEmpty() const noexcept;
    constexpr inline bool isValid() const noexcept;
    constexpr inline float &width() noexcept;
    constexpr inline float &height() noexcept;
    constexpr inline float width() const noexcept;
    constexpr inline float height() const noexcept;
    constexpr inline void setWidth(float w) noexcept;
    constexpr inline void setHeight(float h) noexcept;
    constexpr void transpose() noexcept;
    constexpr inline Size transposed() const noexcept;
    constexpr void scale(float w, float h, AspectRatioMode mode = IgnoreAspectRatio) noexcept;
    constexpr void scale(const Size &s, AspectRatioMode mode = IgnoreAspectRatio) noexcept;
    constexpr Size scaled(float w, float h, AspectRatioMode mode = IgnoreAspectRatio) const noexcept;
    constexpr Size scaled(const Size &s, AspectRatioMode mode = IgnoreAspectRatio) const noexcept;
    constexpr inline Size expandedTo(const Size &) const noexcept;
    constexpr inline Size boundedTo(const Size &) const noexcept;
    constexpr inline Size &operator+=(const Size &) noexcept;
    constexpr inline Size &operator-=(const Size &) noexcept;
    constexpr inline Size &operator*=(float c) noexcept;
    constexpr inline const Size operator+(const Size &) noexcept;
    constexpr inline const Size operator-(const Size &) noexcept;
    constexpr inline const Size operator*(float) noexcept;
    constexpr inline const Size operator*(const Size &) noexcept;
    constexpr inline Size &operator/=(float c);
    constexpr inline const Size operator/(float);
private:
    float m_width;
    float m_height;
};


}

#include <TunisSize.inl>

#endif // TUNISSIZE_H
