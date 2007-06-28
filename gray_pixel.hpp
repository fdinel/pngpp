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
#include "packed_pixel.hpp"
#include "pixel_traits.hpp"

namespace png
{

    typedef byte gray_pixel;
    typedef uint_16 gray_pixel_16;

    template< int bits >
    class packed_gray_pixel
        : public packed_pixel< bits >
    {
    public:
        explicit packed_gray_pixel(byte val = 0)
            : packed_pixel< bits >(val)
        {
        }
    };

    typedef packed_gray_pixel< 1 > gray_pixel_1;
    typedef packed_gray_pixel< 2 > gray_pixel_2;
    typedef packed_gray_pixel< 4 > gray_pixel_4;

    /**
     * \brief  Pixel traits specialization for gray_pixel.
     */
    template<>
    struct pixel_traits< gray_pixel >
        : basic_pixel_traits< byte, color_type_gray, 1 >
    {
    };

    /**
     * \brief  Pixel traits specialization for gray_pixel_16.
     */
    template<>
    struct pixel_traits< gray_pixel_16 >
        : basic_pixel_traits< uint_16, color_type_gray, 1 >
    {
    };

    /**
     * \brief  Pixel traits specialization for packed_gray_pixel.
     */
    template< int bits >
    struct pixel_traits< packed_gray_pixel< bits > >
        : basic_pixel_traits< byte, color_type_gray, 1, bits >
    {
    };

} // namespace png

#endif // PNGPP_GRAY_PIXEL_HPP_INCLUDED
