#ifndef TUNISSOA_H
#define TUNISSOA_H

#include <soa.h>

namespace tunis

{

template <typename... Elements>
class SOA
{
public:
    using RefCount = size_t;
    enum {_refCount = sizeof...(Elements) };

    SOA();
    SOA(const SOA &other);
    ~SOA();
    SOA &operator=(const SOA &other);

    template <size_t ArrayIndex>
    typename SoA<Elements...>::NthTypeOf<ArrayIndex>& get() const;

    static void reserve(size_t size);
    static SoA<Elements..., RefCount> _soa;
    static std::vector<size_t> _available;

protected:

    size_t id;

};

}

#include <TunisSOA.inl>

#endif // TUNISSOA_H
