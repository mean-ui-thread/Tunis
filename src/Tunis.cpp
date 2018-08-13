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

void Context::beginFrame(int32_t windowWidth, int32_t windowHeight)
{
    m_pBackend->renderState.viewport = glm::ivec4(0, 0, windowWidth, windowHeight);


}

void Context::endFrame()
{
    m_pBackend->renderState.apply();

}
