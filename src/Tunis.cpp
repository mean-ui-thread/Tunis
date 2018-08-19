#include <Tunis.h>

#include "TunisBackend.h"

using namespace tunis;

Context::Context() :
    fillStyle(Paint(color::Black)),
    m_pBackend(new Backend())
{
    int32_t maxTexSize = m_pBackend->getMaxTextureSize();
    m_pBackend->renderState.texture = Texture(maxTexSize, maxTexSize);

}

Context::~Context()
{

}

void Context::setBackgroundColor(const glm::vec4 &color)
{
    m_pBackend->renderState.clearColor = color;
}

void Context::beginFrame(int32_t x, int32_t y, int32_t w, int32_t h)
{
    m_pBackend->renderState.viewport = glm::ivec4(x, y, w, h);
    m_pBackend->clearFrame();
    m_pBackend->vertexBuffer.clear();
}

void Context::endFrame()
{
    m_pBackend->renderState.sync();
}

void Context::fillRect(float x, float y, float width, float height)
{
    size_t i = m_pBackend->vertexBuffer.size();

    // allocate room for 4 new vertices.
    m_pBackend->vertexBuffer.resize(i + 4);

    float one = m_pBackend->getTextelSize();

    // top left
    m_pBackend->vertexBuffer[i].pos.x = x;
    m_pBackend->vertexBuffer[i].pos.y = y;
    m_pBackend->vertexBuffer[i].tcoord.s = 0;
    m_pBackend->vertexBuffer[i].tcoord.t = 0;
    m_pBackend->vertexBuffer[i].color = fillStyle.getInnerColor();
    ++i;

    // top right
    m_pBackend->vertexBuffer[i].pos.x = x + width;
    m_pBackend->vertexBuffer[i].pos.y = y;
    m_pBackend->vertexBuffer[i].tcoord.s = one;
    m_pBackend->vertexBuffer[i].tcoord.t = 0;
    m_pBackend->vertexBuffer[i].color = fillStyle.getInnerColor();
    ++i;

    // bottom right
    m_pBackend->vertexBuffer[i].pos.x = x + width;
    m_pBackend->vertexBuffer[i].pos.y = y + height;
    m_pBackend->vertexBuffer[i].tcoord.s = one;
    m_pBackend->vertexBuffer[i].tcoord.t = one;
    m_pBackend->vertexBuffer[i].color = fillStyle.getInnerColor();
    ++i;

    // bottom left
    m_pBackend->vertexBuffer[i].pos.x = x;
    m_pBackend->vertexBuffer[i].pos.y = y + height;
    m_pBackend->vertexBuffer[i].tcoord.s = 0;
    m_pBackend->vertexBuffer[i].tcoord.t = one;
    m_pBackend->vertexBuffer[i].color = fillStyle.getInnerColor();
    ++i;


}
