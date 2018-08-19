#include <TunisBackend.h>

#include "TunisGL.h"

#include <cassert>

using namespace tunis;

Backend::Backend()
{
    vertexBuffer.reserve(4096);

    auto tunisGL_initialized = tunisGLInit();
    assert(KHRONOS_TRUE == tunisGL_initialized);
}

Backend::~Backend()
{
    tunisGLShutdown();
}

void Backend::clearFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

float Backend::getTextelSize() const
{
    static float s_textelWidth = 1.0f / getMaxTextureSize();
    return s_textelWidth;
}

int32_t Backend::getMaxTextureSize() const
{
    static int32_t s_maxTextureSize = 0;
    if (s_maxTextureSize == 0)
    {
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &s_maxTextureSize);
    }
    return s_maxTextureSize;
}

int32_t Backend::getTexturePadding() const
{
    static int32_t s_texturePadding = 16; // should be enough I hope...
    return s_texturePadding;
}
