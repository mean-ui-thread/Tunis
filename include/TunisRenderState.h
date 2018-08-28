#ifndef TUNISRENDERSTATE_H
#define TUNISRENDERSTATE_H

#include <vector>
#include <glm/vec4.hpp>
#include "TunisTexture.h"

namespace tunis
{

template<typename Ttype>
class RenderState
{
public:
    RenderState()
    {
    }

    RenderState(const Ttype& value) :
        m_value(value)
    {
    }

    RenderState(const RenderState& other) :
        m_value(other.m_value)
    {
    }

    inline Ttype& operator=(const Ttype& value)
    {
        if (value != m_value)
        {
            m_value = value;
            m_isDirty = true;
        }
        return m_value;
    }

    inline RenderState& operator=(const RenderState& other)
    {
        *this = other.m_value;
        m_stack = other.m_stack;
        return *this;
    }

    inline const Ttype& get() const
    {
        return m_value;
    }

    inline operator const Ttype&() const
    {
        return m_value;
    }

    inline Ttype* operator-> ()
    {
        return &m_value;
    }

    inline bool isDirty() const
    {
        return m_isDirty;
    }

    inline void setClean()
    {
        m_isDirty = false;
    }

    inline void setDirty()
    {
        m_isDirty = true;
    }

    inline void reset()
    {
        if (!m_stack.empty())
        {
            *this = m_stack.front();
            m_stack.clear();
        }
        m_isDirty = true;
    }

    inline void reset(const Ttype& value)
    {
        *this = value;
        m_stack.clear();
        m_isDirty = true;
    }

    inline void push(const Ttype& value)
    {
        m_stack.push_back(m_value);
        *this = value;
    }

    inline void pop()
    {
        if (!m_stack.empty())
        {
            *this = m_stack.back();
            m_stack.pop_back();
        }
    }

    inline size_t size() const
    {
        return m_stack.size();
    }

private:
    bool m_isDirty = false;
    std::vector<Ttype> m_stack;
    Ttype m_value;
};

}


#endif // TUNISRENDERSTATE_H
