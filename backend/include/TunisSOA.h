#ifndef TUNISSOA_H
#define TUNISSOA_H

#include <soa.h>


#define TUNIS_SOA_DEFAULT_DECL(_SOA_CLASS_)                                    \
public:                                                                        \
    _SOA_CLASS_();                                                             \
    _SOA_CLASS_(const _SOA_CLASS_ &other);                                     \
    ~_SOA_CLASS_();                                                            \
    _SOA_CLASS_ &operator=(const _SOA_CLASS_ &other);                          \
private:                                                                       \
    size_t id                                                                  \



#define TUNIS_SOA_DEFAULT_INL(_SOA_CLASS_)                                     \
                                                                               \
inline _SOA_CLASS_::_SOA_CLASS_()                                              \
{                                                                              \
    if (_available.size() > 0)                                                 \
    {                                                                          \
        id = _available.back();                                                \
        _available.pop_back();                                                 \
    }                                                                          \
    else                                                                       \
    {                                                                          \
        id = _soa.size();                                                      \
        _soa.resize(_soa.size()+1);                                            \
    }                                                                          \
}                                                                              \
                                                                               \
inline _SOA_CLASS_::_SOA_CLASS_(const _SOA_CLASS_ &other) : id(other.id)       \
{                                                                              \
    ++_soa.get<_refCount>(id);                                                 \
}                                                                              \
                                                                               \
inline _SOA_CLASS_::~_SOA_CLASS_()                                             \
{                                                                              \
    if (--_soa.get<_refCount>(id) == 0)                                        \
    {                                                                          \
        _available.push_back(id);                                              \
    }                                                                          \
}                                                                              \
                                                                               \
inline _SOA_CLASS_ &_SOA_CLASS_::operator=(const _SOA_CLASS_ &other)           \
{                                                                              \
    if (this != &other)                                                        \
    {                                                                          \
        int refCount = _soa.get<_refCount>(id);                                \
        _soa.copy(other.id, id);                                               \
        _soa.get<_refCount>(id) = refCount;                                    \
    }                                                                          \
                                                                               \
    return *this;                                                              \
}                                                                              \

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
