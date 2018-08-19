#ifndef TUNISPAINT_H
#define TUNISPAINT_H

#include "TunisColor.h"
#include "soa.h"

#include <algorithm>
#include <array>

namespace tunis
{

class Paint
{
public:

    inline static Paint createLinearGradient(float startX, float startY,
                                             float endX, float endY,
                                             Color innerColor,
                                             Color outerColor)
    {
        float dx, dy, d;
        const float large = 1e5;
        // Calculate transform aligned to the line
        dx = endX - startX;
        dy = endY - startY;
        d = sqrtf(dx*dx + dy*dy);
        if (d > 0.0001f)
        {
            dx /= d;
            dy /= d;
        }
        else
        {
            dx = 0;
            dy = 1;
        }

        size_t id;

        if (_available.size() > 0)
        {
            id = _available.back();
            _available.pop_back();
        }
        else
        {
            id = _soa.size();
            _soa.resize(_soa.size()+1);
        }

        _soa.get<_xform>(id) = {dy, -dx, dx, dy, startX - dx*large, startY - dy*large};
        _soa.get<_extend>(id) = {large, large + d*0.5f};
        _soa.get<_radius>(id) = 0.0f;
        _soa.get<_feather>(id) = std::max(1.0f, d);
        _soa.get<_innerColor>(id) = innerColor;
        _soa.get<_outerColor>(id) = outerColor;
        _soa.get<_image>(id) = 0;
        _soa.get<_refCount>(id) = 1;

        return Paint(id);
    }

    inline Paint(const Color &color)
    {

        if (_available.size() > 0)
        {
            id = _available.back();
            _available.pop_back();
        }
        else
        {
            id = _soa.size();
            _soa.resize(_soa.size()+1);
        }

        _soa.get<_xform>(id) = {};
        _soa.get<_extend>(id) = {};
        _soa.get<_radius>(id) = 0;
        _soa.get<_feather>(id) = 0;
        _soa.get<_innerColor>(id) = color;
        _soa.get<_outerColor>(id) = color::Transparent;
        _soa.get<_image>(id) = 0;
        _soa.get<_refCount>(id) = 1;

    }

    inline Paint(const Paint &other) : id(other.id)
    {
        ++_soa.get<_refCount>(id);
    }

    inline ~Paint()
    {
        if (--_soa.get<_refCount>(id) == 0)
        {
            _available.push_back(id);
        }
    }

    inline Paint &operator=(const Paint &other)
    {
        if (this != &other)
        {
            int refCount = _soa.get<_refCount>(id);
            _soa.copy(other.id, id);
            _soa.get<_refCount>(id) = refCount;
        }

        return *this;
    }

    inline Paint &operator=(const Color &color)
    {
        _soa.get<_xform>(id) = {};
        _soa.get<_extend>(id) = {};
        _soa.get<_radius>(id) = 0;
        _soa.get<_feather>(id) = 0;
        _soa.get<_innerColor>(id) = color;
        _soa.get<_outerColor>(id) = color::Transparent;
        _soa.get<_image>(id) = 0;

        return *this;
    }

    inline const std::array<float, 6> &getXform() const
    {
        return _soa.get<_xform>(id);
    }

    inline const std::array<float, 2> &getExtend() const
    {
        return _soa.get<_extend>(id);
    }

    inline float getRadius() const
    {
        return _soa.get<_radius>(id);
    }

    inline float getFeather() const
    {
        return _soa.get<_feather>(id);
    }

    inline Color getInnerColor() const
    {
        return _soa.get<_innerColor>(id);
    }

    inline Color getOuterColor() const
    {
        return _soa.get<_outerColor>(id);
    }

    inline size_t getImage() const
    {
        return _soa.get<_image>(id);
    }

    inline int getRefCount() const
    {
        return _soa.get<_refCount>(id);
    }

private:

    inline Paint(size_t id) : id(id)
    {
    }

    enum {
        _xform = 0,
        _extend,
        _radius,
        _feather,
        _innerColor,
        _outerColor,
        _image,
        _refCount
    };

    static SoA<
        std::array<float, 6>, // _xform
        std::array<float, 2>, // _extend
        float,                // _radius
        float,                // _feather
        Color,                // _innerColor
        Color,                // _outerColor
        size_t,               // _image
        int                   // _refCount
    > _soa;

    static std::vector<size_t> _available;

    size_t id;
};

}

#endif // TUNISPAINT_H
