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


private:
    std::unique_ptr<Backend> m_pBackend;
};

}

#endif // TUNIS_H
