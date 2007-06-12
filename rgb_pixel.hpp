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
#ifndef PNGPP_RGB_PIXEL_HPP_INCLUDED
#define PNGPP_RGB_PIXEL_HPP_INCLUDED

#include "types.hpp"
#include "pixel_traits.hpp"

namespace png
{

    /**
     * \brief  RGB pixel type.
     */
    struct rgb_pixel
    {
        /**
         * \brief  Default constructor.  Initializes all components
         * with zeros.
         */
        rgb_pixel()
            : red(0), green(0), blue(0)
        {
        }

        /**
         * \brief  Constructs rgb_pixel object from \a red, \a green
         * and \a blue components passed as parameters.
         */
        rgb_pixel(byte red, byte green, byte blue)
            : red(red), green(green), blue(blue)
        {
        }

        byte red;
        byte green;
        byte blue;
    };

    /**
     * \brief  Pixel traits specialization for rgb_pixel.
     */
    template<>
    struct pixel_traits< rgb_pixel >
    {
        static color_type get_color_type()
        {
            return color_type_rgb;
        }

        static int get_bit_depth()
        {
            return 8;
        }
    };

} // namespace png

#endif // PNGPP_RGB_PIXEL_HPP_INCLUDED
