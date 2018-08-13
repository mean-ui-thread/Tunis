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
