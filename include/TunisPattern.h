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
#ifndef TUNISPATTERN_H
#define TUNISPATTERN_H

#include <TunisTypes.h>
#include <TunisImage.h>

namespace tunis
{
    enum class RepeatType
    {
        repeat,    // both directions
        repeat_x,  // horizontal only
        repeat_y,  // vertical only
        no_repeat, // neither
    };

    /*!
     * \brief The Pattern class represents an opaque object describing a
     * pattern, based on an image, created by the
     * tunis::Context.createPattern() method.
     *
     * \note It can be used as a fillStyle or strokeStyle.
     */
    class Pattern : public RefCountedSOA<Image, RepeatType>
    {
        inline Image &image() { return get<0>(); }
        inline RepeatType &repetition() { return get<1>(); }

        inline const Image &image() const { return get<0>(); }
        inline const RepeatType &repetition() const { return get<1>(); }

        friend class Context;
        friend class Paint;

        Pattern(Image image, RepeatType repetition);
    };

}


#include "TunisPattern.inl"

#endif // TUNISPATTERN_H
