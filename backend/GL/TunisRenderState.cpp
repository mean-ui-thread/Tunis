#include <TunisRenderState.h>

#include <TunisGL.h>

using namespace tunis;

RenderState::RenderState() :
    clearColor(glm::vec4(0, 0, 0, 0)),
    viewport(glm::ivec4(0, 0, 0, 0))
{
}

RenderState::RenderState(const RenderState& other)
{
    clearColor = other.clearColor;
    viewport = other.viewport;
}

RenderState& RenderState::operator=(const RenderState& other)
{
    if (&other != this)
    {
        clearColor = other.clearColor;
        viewport = other.viewport;
    }

    return *this;
}

void RenderState::reset()
{
    clearColor.reset();
    viewport.reset();
}

void RenderState::apply()
{
    if (clearColor.isDirty())
    {
        const glm::vec4& color = clearColor.get();
        glClearColor(color.r, color.g, color.b, color.a);
        clearColor.setClean();
    }

    if (viewport.isDirty())
    {
        const glm::ivec4& rect = viewport.get();
        glViewport(rect.x, rect.y, rect.z, rect.w);
        viewport.setClean();
    }
}
