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
