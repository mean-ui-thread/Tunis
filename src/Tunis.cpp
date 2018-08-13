#include <Tunis.h>

#include "TunisBackend.h"

using namespace tunis;

Context::Context() :
    m_pBackend(new Backend())
{

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

}

void Context::endFrame()
{
    m_pBackend->renderState.apply();

}
