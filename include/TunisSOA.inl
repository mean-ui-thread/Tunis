#include <TunisSOA.h>

namespace tunis
{

template <typename... Elements>
inline RefCountedSOA<Elements...>::RefCountedSOA()
{
    if (_available.size() > 0)
    {
        _id = _available.back();
        _available.pop_back();
    }
    else
    {
        _id = _soa.size();
        _soa.resize(_soa.size()+1);
    }

    _soa.get<_refCount>(_id) = 1;
}

template <typename... Elements>
inline RefCountedSOA<Elements...>::RefCountedSOA(const RefCountedSOA &other) :
    _id(other._id)
{
    ++_soa.get<_refCount>(_id);
}

template <typename... Elements>
inline RefCountedSOA<Elements...>::~RefCountedSOA()
{
    if (--_soa.get<_refCount>(_id) == 0)
    {
        _available.push_back(_id);
    }
}

template <typename... Elements>
inline RefCountedSOA<Elements...> &RefCountedSOA<Elements...>::operator=(const RefCountedSOA &other)
{
    if (this != &other)
    {
        if (--_soa.get<_refCount>(_id) == 0)
        {
            _available.push_back(_id);
        }

        _id = other._id;

        ++_soa.get<_refCount>(_id);
    }

    return *this;
}

template <typename... Elements>
template <size_t ArrayIndex>
inline typename SoA<Elements...>::NthTypeOf<ArrayIndex>& RefCountedSOA<Elements...>::get() const
{
    return _soa.get<ArrayIndex>(_id);
}


template <typename... Elements>
inline void RefCountedSOA<Elements...>::reserve(size_t size)
{
    _soa.reserve(size);
}

template <typename... Elements>
SoA<Elements..., typename RefCountedSOA<Elements...>::RefCount> RefCountedSOA<Elements...>::_soa;

template <typename... Elements>
std::vector<size_t> RefCountedSOA<Elements...>::_available;


}


