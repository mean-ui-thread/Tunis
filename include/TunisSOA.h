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
#ifndef TUNISSOA_H
#define TUNISSOA_H

#include <soa.h>

namespace tunis
{
    /*!
     * This is a simple helper class that automatically gives you a slice of an
     * internal static SOA, including a reference count. Once the reference counter
     * reaches 0, the instance is *not* deleted, but added to a pool under a static
     * _available and the SOA slice will be reused on the next stack allocation.
     *
     * In other word, a class instance becomes a simple SoA index wrapper, and the
     * users uses that class instance like a regular object without kowing that it
     * is an SOA slice wrapper.
     *
     * Deriving from it should help to greatly reducing allocations.
     *
     * Recommended usage: simply replace
     *
     * struct MyVector
     * {
     *     float x;
     *     float y;
     * };
     *
     * by:
     *
     * struct MyVector : public RefCountedSOA<float, float>
     * {
     *     inline float &x() { return get<0>(); }
     *     inline float &y() { return get<1>(); }
     * };
     *
     * And every instance will behave pretty much like a dereferenced shared pointer
     * with minimal cost.
     *
     * Of course, the example above is a bad example, as MyVector too small to be
     * worth ref-counting and pooling on destruction.
     *
     * If I don't need any reference counting and pooling, but asolutely need an
     * array of my struct, instead of doing std::vector<MyVector>, I prefer using
     * this approach instead:
     *
     * struct MyVectorArray : public SoA<float, float>
     * {
     *     inline float &x(size_t idx) { return get<0>(idx); }
     *     inline float &y(size_t idx) { return get<1>(idx); }
     * };
     *
     * (notice that I renamed MyVector to MyVectorArray for better clarity)
     *
     * if done correctly, you shouldn't need to use std::vector anywhere.
     */
    template <typename... Elements>
    class RefCountedSOA
    {
    public:
        using refid_t = uint32_t;
        using refcount_t = uint32_t;
        enum {_refCount = sizeof...(Elements) };

        RefCountedSOA();
        RefCountedSOA(const RefCountedSOA &other);
        RefCountedSOA(RefCountedSOA &&other);
        ~RefCountedSOA();
        RefCountedSOA &operator=(const RefCountedSOA &other);
        RefCountedSOA &operator=(RefCountedSOA &&other);

        bool operator==(const RefCountedSOA &other);
        bool operator!=(const RefCountedSOA &other);

        refid_t getId() const;

        template <typename T> T clone();

        static void reserve(size_t size);

    protected:

        /*!
         * convienient function that pretty much does _soa.get<IDX>(_id); for you.
         */
        template <size_t ArrayIndex>
        typename SoA<Elements..., refcount_t>::template NthTypeOf<ArrayIndex>& get() const;

    private:
        static SoA<Elements..., refcount_t> _soa;
        static std::vector<refid_t> _available;
        refid_t _id;
    };

}

#include <TunisSOA.inl>

#endif // TUNISSOA_H
