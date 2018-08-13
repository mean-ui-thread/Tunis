#ifndef TUNIS_H
#define TUNIS_H

#include <memory>
#include "TunisColor.h"

namespace tunis
{

class Backend;

class Context
{
public:
    Context();
    ~Context();

    void setBackgroundColor(const glm::vec4 &color);
    void beginFrame(int32_t x, int32_t y, int32_t w, int32_t h);
    void endFrame();

private:
    std::unique_ptr<Backend> m_pBackend;
};

}

#endif // TUNIS_H
