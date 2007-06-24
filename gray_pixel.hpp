/*
 * Copyright (C) 2007   Alex Shulgin
 *
 * This file is part of png++ the C++ wrapper for libpng.  Png++ is free
 * software; the exact copying conditions are as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 3. The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef PNGPP_GRAY_PIXEL_HPP_INCLUDED
#define PNGPP_GRAY_PIXEL_HPP_INCLUDED

#include "types.hpp"
#include "pixel_traits.hpp"

namespace png
{

    namespace
    {
        template< typename T, int bits > class allowed_bit_depth;

        template<> class allowed_bit_depth< byte, 1 > {};
        template<> class allowed_bit_depth< byte, 2 > {};
        template<> class allowed_bit_depth< byte, 4 > {};
        template<> class allowed_bit_depth< byte, 8 > {};
//        template<> class allowed_bit_depth< uint_16, 16 > {};
    }

    template< typename T, int bits = sizeof(T) * 8 >
    class basic_gray_pixel
        : allowed_bit_depth< T, bits >
    {
    public:
        explicit basic_gray_pixel(T val = 0)
        {
            *this = val;
        }

        basic_gray_pixel< T, bits >& operator=(T val)
        {
            value = val & bit_mask;
            return *this;
        }

    private:
//        static int const bit_depth = bits;
        static T const bit_mask = (1 << bits) - 1;

        T value;
    };

    /**
     * \brief  Gray pixel type.
     */
    typedef basic_gray_pixel< byte > gray_pixel;

    /**
     * \brief  Pixel traits specialization for gray_pixel.
     */
    template< typename T, int bits >
    struct pixel_traits< basic_gray_pixel< T, bits > >
        : basic_pixel_traits< T, color_type_gray, bits >
    {
    };

} // namespace png

#endif // PNGPP_GRAY_PIXEL_HPP_INCLUDED
