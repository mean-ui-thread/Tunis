#include <TunisRenderState.h>

#include <TunisGL.h>

using namespace tunis;

RenderState::RenderState() :
    viewport(glm::ivec4(0, 0, 0, 0))
{
}

RenderState::RenderState(const RenderState& other)
{
    viewport = other.viewport;
}

RenderState& RenderState::operator=(const RenderState& other)
{
    if (&other != this)
    {
        viewport = other.viewport;
    }

    return *this;
}


void RenderState::reset()
{
    viewport.reset();
}

void RenderState::apply()
{
    if (viewport.isDirty())
    {
        const glm::ivec4& rect = viewport.get();
        glViewport(rect.x, rect.y, rect.z, rect.w);
        viewport.resetDirty();
    }
}
