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
#ifndef TUNISBITMASK_H
#define TUNISBITMASK_H

#include <type_traits>

namespace tunis
{
    namespace detail
    {
        template <typename E>
        class BitMask
        {
            typedef typename std::underlying_type<E>::type EType;
            EType value;
        public:
            BitMask() :
                value(0)
            {
            }

            BitMask(E e) :
                value(static_cast<EType>(e))
            {
            }

            BitMask(const BitMask &) = default;
            BitMask &operator=(const BitMask &) = default;

            void operator=(E e)
            {
                value = static_cast<EType>(e);
            }

            void set(E e)
            {
                value |= static_cast<EType>(e);
            }

            void reset(E e)
            {
                value &= ~static_cast<EType>(e);
            }

            bool test(E e) const
            {
                return value & static_cast<EType>(e);
            }

            operator EType() const { return value; }
        };
    }

}


#endif // TUNISBITMASK_H
