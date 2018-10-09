/**
 * MIT License
 *
 * Copyright (c) 2018 Matt Chiasson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/
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
            _id = static_cast<id_t>(_soa.size());
            _soa.resize(_soa.size()+1);
        }

        _soa.template get<_refCount>(_id) = 1;
    }

    template <typename... Elements>
    inline RefCountedSOA<Elements...>::RefCountedSOA(const RefCountedSOA &other) :
        _id(other._id)
    {
        ++_soa.template get<_refCount>(_id);
    }

    template <typename... Elements>
    inline RefCountedSOA<Elements...>::RefCountedSOA(RefCountedSOA &&other) :
        _id(std::move(other._id))
    {
        ++_soa.template get<_refCount>(_id);
    }

    template <typename... Elements>
    inline RefCountedSOA<Elements...>::~RefCountedSOA()
    {
        if (--_soa.template get<_refCount>(_id) == 0)
        {
            _available.push_back(_id);
        }
    }

    template <typename... Elements>
    inline RefCountedSOA<Elements...> &RefCountedSOA<Elements...>::operator=(const RefCountedSOA &other)
    {
        if (this != &other)
        {
            if (--_soa.template get<_refCount>(_id) == 0)
            {
                _available.push_back(_id);
            }

            _id = other._id;

            ++_soa.template get<_refCount>(_id);
        }

        return *this;
    }

    template <typename... Elements>
    inline RefCountedSOA<Elements...> &RefCountedSOA<Elements...>::operator=(RefCountedSOA &&other)
    {
        if (this != &other)
        {
            if (--_soa.template get<_refCount>(_id) == 0)
            {
                _available.push_back(_id);
            }

            _id = std::move(other._id);

            ++_soa.template get<_refCount>(_id);
        }

        return *this;
    }

    template <typename... Elements>
    bool RefCountedSOA<Elements...>::operator==(const RefCountedSOA &other)
    {
        return _id == other._id;
    }

    template <typename... Elements>
    bool RefCountedSOA<Elements...>::operator!=(const RefCountedSOA &other)
    {
        return _id != other._id;
    }

    template <typename... Elements>
    id_t RefCountedSOA<Elements...>::getId() const
    {
        return _id;
    }

    template <typename... Elements>
    template <typename T>
    inline T RefCountedSOA<Elements...>::clone()
    {
        T instance;

        _soa.copy(_id, instance._id);

        // since SoA::copy copies every fields cluding the refCount fields of the
        // current instance, we need to manually reset our new instance's refcount
        // field since there are no way to tell SoA::copy to exclude fields.
        _soa.template get<_refCount>(instance._id) = 1;

        return std::move(instance);
    }

    template <typename... Elements>
    template <size_t ArrayIndex>
    inline typename SoA<Elements..., typename RefCountedSOA<Elements...>::refcount_t>::template NthTypeOf<ArrayIndex>& RefCountedSOA<Elements...>::get() const
    {
        return _soa.template get<ArrayIndex>(_id);
    }


    template <typename... Elements>
    inline void RefCountedSOA<Elements...>::reserve(size_t size)
    {
        _soa.reserve(size);
    }

    template <typename... Elements>
    SoA<Elements..., typename RefCountedSOA<Elements...>::refcount_t> RefCountedSOA<Elements...>::_soa;

    template <typename... Elements>
    std::vector<typename RefCountedSOA<Elements...>::id_t> RefCountedSOA<Elements...>::_available;


}


