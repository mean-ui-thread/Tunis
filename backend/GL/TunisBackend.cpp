#include <TunisBackend.h>

#include "TunisGL.h"

#include <cassert>

using namespace tunis;

Backend::Backend()
{
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
