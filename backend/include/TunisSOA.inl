#include <TunisSOA.h>

namespace tunis
{

template <typename... Elements>
inline SOA<Elements...>::SOA()
{
    if (_available.size() > 0)
    {
        id = _available.back();
        _available.pop_back();
    }
    else
    {
        id = _soa.size();
        _soa.resize(_soa.size()+1);
    }
}

template <typename... Elements>
inline SOA<Elements...>::SOA(const SOA &other) :
    id(other.id)
{
    ++_soa.get<_refCount>(id);
}

template <typename... Elements>
inline SOA<Elements...>::~SOA()
{
    if (--_soa.get<_refCount>(id) == 0)
    {
        _available.push_back(id);
    }
}

template <typename... Elements>
inline SOA<Elements...> &SOA<Elements...>::operator=(const SOA &other)
{
    if (this != &other)
    {
        int refCount = _soa.get<_refCount>(id); //backup refCount
        _soa.copy(other.id, id);
        _soa.get<_refCount>(id) = refCount; //restore refCount
    }

    return *this;
}

template <typename... Elements>
template <size_t ArrayIndex>
typename SoA<Elements...>::NthTypeOf<ArrayIndex>& SOA<Elements...>::get() const
{
    return _soa.get<ArrayIndex>(id);
}


template <typename... Elements>
void SOA<Elements...>::reserve(size_t size)
{
    _soa.reserve(size);
}

template <typename... Elements>
SoA<Elements..., typename SOA<Elements...>::RefCount> SOA<Elements...>::_soa;

template <typename... Elements>
std::vector<size_t> SOA<Elements...>::_available;


}


