#include "TunisPaint.h"

using namespace tunis;

SoA<
    std::array<float, 6>,
    std::array<float, 2>,
    float,
    float,
    Color,
    Color,
    size_t,
    int
> Paint::_soa;

std::vector<size_t> Paint::_available;
