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
#ifndef PNGPP_REQUIRE_COLOR_SPACE_HPP_INCLUDED
#define PNGPP_REQUIRE_COLOR_SPACE_HPP_INCLUDED

#include "rgb_pixel.hpp"
#include "rgba_pixel.hpp"

namespace png
{

    /**
     * \brief  IO transformation class template.  Enforces image color space.
     *
     * This IO transformation class template used to enforce source image
     * color space.  Not implemented -- see specializations.
     *
     * \see  image, image::read
     * \see  require_color_space<rgb_pixel>, require_color_space<rgba_pixel>
     */
    template< typename pixel >
    struct require_color_space
    {
        void operator()(io_base& io) const;
    };

    /**
     * \brief  Enforces image color space.  Specialization for rgb_pixel pixel
     * type.
     *
     * Checks image color_type and bit depth.  Throws error on mismatch.
     */
    template<>
    struct require_color_space< rgb_pixel >
    {
        void operator()(io_base& io) const
        {
            if (io.get_color_type() != color_type_rgb
                || io.get_bit_depth()
                   != pixel_traits< rgb_pixel >::get_bit_depth())
            {
                throw error("RGB color space required");
            }
        }
    };

    /**
     * \brief  Enforces image color space.  Specialization for rgba_pixel pixel
     * type.
     *
     * Checks image color_type and bit depth.  Throws error on mismatch.
     */
    template<>
    struct require_color_space< rgba_pixel >
    {
        void operator()(io_base& io) const
        {
            if (io.get_color_type() != color_type_rgba
                || io.get_bit_depth()
                   != pixel_traits< rgba_pixel >::get_bit_depth())
            {
                throw error("RGBA color space required");
            }
        }
    };

} // namespace png

#endif // PNGPP_REQUIRE_COLOR_SPACE_HPP_INCLUDED
