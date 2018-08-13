#ifndef TUNISBACKEND_H
#define TUNISBACKEND_H

#include <cinttypes>

#include <TunisRenderState.h>

namespace tunis
{

class Backend
{
public:
    Backend();
    ~Backend();

    RenderState renderState;
};

}

#endif // TUNISBACKEND_H
