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

    void clearFrame();
};

}

#endif // TUNISBACKEND_H
