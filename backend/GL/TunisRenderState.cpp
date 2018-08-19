#include <TunisRenderState.h>

#include <TunisGL.h>

using namespace tunis;

RenderState::RenderState() :
    clearColor(glm::vec4(0, 0, 0, 0)),
    viewport(glm::ivec4(0, 0, 0, 0)),
    texture()
{
}

RenderState::RenderState(const RenderState& other)
{
    clearColor = other.clearColor;
    viewport = other.viewport;
    texture = other.texture;
}

RenderState& RenderState::operator=(const RenderState& other)
{
    if (&other != this)
    {
        clearColor = other.clearColor;
        viewport = other.viewport;
        texture = other.texture;
    }

    return *this;
}

void RenderState::reset()
{
    clearColor.reset(glm::vec4(0, 0, 0, 0));
    viewport.reset(glm::ivec4(0, 0, 0, 0));
    texture.reset(Texture());
}

void RenderState::sync()
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

    if (texture.isDirty())
    {
        texture->bind();
        texture.setClean();
    }

}
