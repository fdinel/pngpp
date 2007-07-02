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

#include "error.hpp"
#include "rgb_pixel.hpp"
#include "rgba_pixel.hpp"
#include "gray_pixel.hpp"
#include "ga_pixel.hpp"
#include "io_base.hpp"

namespace png
{

    namespace
    {

        template< typename pixel >
        struct wrong_color_space
        {
            static char const* error_msg;
        };

        template<> char const* wrong_color_space< rgb_pixel >::error_msg =
            "RGB color space required";

        template<> char const* wrong_color_space< rgba_pixel >::error_msg =
            "RGBA color space required";

        template<> char const* wrong_color_space< gray_pixel >::error_msg =
            "Grayscale color space required";

        template<> char const* wrong_color_space< gray_pixel_1 >::error_msg =
            "1-bit Grayscale color space required";

        template<> char const* wrong_color_space< gray_pixel_2 >::error_msg =
            "2-bit Grayscale color space required";

        template<> char const* wrong_color_space< gray_pixel_4 >::error_msg =
            "4-bit Grayscale color space required";

        template<> char const* wrong_color_space< ga_pixel >::error_msg =
            "Gray+Alpha color space required";

    } // unnamed namespace

    /**
     * \brief  IO transformation class template.  Enforces image color space.
     *
     * This IO transformation class template used to enforce source image
     * color space.
     *
     * \see  image, image::read
     */
    template< typename pixel >
    struct require_color_space
    {
        void operator()(io_base& io) const
        {
            if (io.get_color_type() != pixel_traits< pixel >::color_space
                || io.get_bit_depth() != pixel_traits< pixel >::bit_depth)
            {
                throw error(wrong_color_space< pixel >::error_msg);
            }
        }
    };

} // namespace png

#endif // PNGPP_REQUIRE_COLOR_SPACE_HPP_INCLUDED
