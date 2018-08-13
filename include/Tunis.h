#ifndef TUNIS_H
#define TUNIS_H

#include <memory>

namespace tunis
{

class Backend;

class Context
{
public:
    Context();
    ~Context();

    void beginFrame(int32_t windowWidth, int32_t windowHeight);
    void endFrame();

private:
    std::unique_ptr<Backend> m_pBackend;
};

}

#endif // TUNIS_H
