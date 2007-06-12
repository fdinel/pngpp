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
#ifndef PNGPP_CONVERT_COLOR_SPACE_HPP_INCLUDED
#define PNGPP_CONVERT_COLOR_SPACE_HPP_INCLUDED

#include "rgb_pixel.hpp"
#include "rgba_pixel.hpp"

namespace png
{

    /**
     * \brief  IO transformation class template.  Converts image color space.
     *
     * This IO transformation class template is used to convert color space of
     * the source image to color space of target image.  Not implemented --
     * see specializations.
     *
     * Specializations try to convert source color space to RGB.  An
     * error with human-readable description is thrown when color
     * space could not be converted.  Often, this means that you have
     * to recompile libpng with some more conversion options turned
     * on.
     *
     * \see  image, image::read
     * \see  convert_color_space<rgb_pixel>, convert_color_space<rgba_pixel>
     */
    template< typename pixel >
    struct convert_color_space
    {
        void operator()(reader& io) const;
        void operator()(writer& io) const;
    };

    /**
     * \brief  Converts image color space.  Specialization for rgb_pixel pixel
     * type.
     */
    template<>
    struct convert_color_space< rgb_pixel >
    {
        void operator()(reader& io) const
        {
            if (io.get_bit_depth() == 16)
            {
#ifdef PNG_READ_16_TO_8_SUPPORTED
                io.set_strip_16();
#else
                throw error("convert_color_space: expected 8-bit data"
                            " but found 16-bit;"
                            " recompile with PNG_READ_16_TO_8_SUPPORTED");
#endif
            }
            if (io.get_color_type() & color_mask_alpha)
            {
#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
                io.set_strip_alpha();
#else
                throw error("convert_color_space: alpha channel unexpected;"
                            " recompile with PNG_READ_STRIP_ALPHA_SUPPORTED");
#endif
            }
            if (io.get_color_type() == color_type_palette)
            {
#ifdef PNG_READ_EXPAND_SUPPORTED
                io.set_palette_to_rgb();
#else
                throw error("convert_color_space: expected RGB data"
                            " but found indexed colors;"
                            " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
            }
            else if (io.get_color_type() == color_type_gray
                     || io.get_color_type() == color_type_gray_alpha)
            {
                if (io.get_color_type() == color_type_gray
                    && io.get_bit_depth() < 8)
                {
#ifdef PNG_READ_EXPAND_SUPPORTED
                    io.set_gray_1_2_4_to_8();
#else
                    throw error("convert_color_space: expected RGB data"
                                " but found grayscale (< 8-bit) colors;"
                                " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
                }
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
                io.set_gray_to_rgb();
#else
                throw error("convert_color_space: expected RGB data"
                            " but found grayscale colors;"
                            " recompile with PNG_READ_GRAY_TO_RGB_SUPPORTED");
#endif
            }
        }

        void operator()(writer& io) const
        {
            io.set_color_type(pixel_traits< rgb_pixel >::get_color_type());
            io.set_bit_depth(pixel_traits< rgb_pixel >::get_bit_depth());
        }

        void operator()(writer& io, color_type color_space, int bit_depth) const
        {
            if (bit_depth == 16)
            {
                throw error("convert_color_space: expected 8-bit data"
                            " but found 16-bit;"
                            " cannot handle this");
            }
            if (color_space & color_mask_alpha)
            {
#ifdef PNG_WRITE_FILLER_SUPPORTED
                io.set_filler(0, filler_after);
#else
                throw error("convert_color_space: alpha channel unexpected;"
                            " recompile with PNG_WRITE_FILLER_SUPPORTED");
#endif
            }
/*
            if (io.get_color_type() == color_type_palette)
            {
#ifdef PNG_READ_EXPAND_SUPPORTED
                io.set_palette_to_rgb();
#else
                throw error("convert_color_space: expected RGB data"
                            " but found indexed colors;"
                            " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
            }
            else if (io.get_color_type() == color_type_gray
                     || io.get_color_type() == color_type_gray_alpha)
            {
                if (io.get_color_type() == color_type_gray
                    && io.get_bit_depth() < 8)
                {
#ifdef PNG_READ_EXPAND_SUPPORTED
                    io.set_gray_1_2_4_to_8();
#else
                    throw error("convert_color_space: expected RGB data"
                                " but found grayscale (< 8-bit) colors;"
                                " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
                }
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
                io.set_gray_to_rgb();
#else
                throw error("convert_color_space: expected RGB data"
                            " but found grayscale colors;"
                            " recompile with PNG_READ_GRAY_TO_RGB_SUPPORTED");
#endif
            }
*/
        }
    };

    /**
     * \brief  Converts image color space.  Specialization for rgba_pixel pixel
     * type.
     */
    template<>
    struct convert_color_space< rgba_pixel >
    {
        explicit convert_color_space(uint_32 filler = 0xff)
            : m_filler(filler)
        {
        }

        void operator()(reader& io) const
        {
            if (io.get_bit_depth() == 16)
            {
#ifdef PNG_READ_16_TO_8_SUPPORTED
                io.set_strip_16();
#else
                throw error("convert_color_space: expected 8-bit data"
                            " but found 16-bit;"
                            " recompile with PNG_READ_16_TO_8_SUPPORTED");
#endif
            }
            if (!(io.get_color_type() & color_mask_alpha))
            {
#if defined(PNG_READ_FILLER_SUPPORTED) && !defined(PNG_1_0_X)
                io.set_add_alpha(m_filler, filler_after);
#else
                throw error("convert_color_space: expected alpha channel"
                            " but none found;"
                            " recompile with PNG_READ_FILLER_SUPPORTED"
                            " and be sure to use libpng > 1.0.x");
#endif
            }
            if (io.get_color_type() == color_type_palette)
            {
#ifdef PNG_READ_EXPAND_SUPPORTED
                io.set_palette_to_rgb();
#else
                throw error("convert_color_space: expected RGB data"
                            " but found indexed colors;"
                            " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
            }
            else if (io.get_color_type() == color_type_gray
                    || io.get_color_type() == color_type_gray_alpha)
            {
                if (io.get_color_type() == color_type_gray
                    && io.get_bit_depth() < 8)
                {
#ifdef PNG_READ_EXPAND_SUPPORTED
                    io.set_gray_1_2_4_to_8();
#else
                    throw error("convert_color_space: expected RGBA data"
                                " but found grayscale (< 8-bit) colors;"
                                " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
                }
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
                io.set_gray_to_rgb();
#else
                throw error("convert_color_space: expected RGBA data"
                            " but found grayscale colors;"
                            " recompile with PNG_READ_GRAY_TO_RGB_SUPPORTED");
#endif
            }
        }

    private:
        uint_32 m_filler;
    };


    /**
     * \brief  Converts image color space.  Specialization for ga_pixel pixel
     * type.
     */
    template<>
    struct convert_color_space< ga_pixel >
    {
        explicit convert_color_space(uint_32 filler = 0xff)
            : m_filler(filler)
        {
        }

        void operator()(reader& io) const
        {
            if (io.get_bit_depth() == 16)
            {
#ifdef PNG_READ_16_TO_8_SUPPORTED
                io.set_strip_16();
#else
                throw error("convert_color_space: expected 8-bit data"
                            " but found 16-bit;"
                            " recompile with PNG_READ_16_TO_8_SUPPORTED");
#endif
            }
            if (!(io.get_color_type() & color_mask_alpha))
            {
#if defined(PNG_READ_FILLER_SUPPORTED) && !defined(PNG_1_0_X)
                io.set_add_alpha(m_filler, filler_after);
#else
                throw error("convert_color_space: expected alpha channel"
                            " but none found;"
                            " recompile with PNG_READ_FILLER_SUPPORTED"
                            " and be sure to use libpng > 1.0.x");
#endif
            }
            if (io.get_color_type() == color_type_palette)
            {
#if defined(PNG_READ_EXPAND_SUPPORTED) \
 && defined(PNG_READ_RGB_TO_GRAY_SUPPORTED)
                io.set_palette_to_rgb();
                io.set_rgb_to_gray();
#else
                throw error("convert_color_space: expected Gray+Alpha data"
                            " but found indexed colors;"
                            " recompile with PNG_READ_EXPAND_SUPPORTED"
                            " && PNG_READ_RGB_TO_GRAY_SUPPORTED");
#endif
            }
            else if (io.get_color_type() == color_type_rgb
                     || io.get_color_type() == color_type_rgba)
            {
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
                io.set_rgb_to_gray();
#else
                throw error("convert_color_space: expected Gray+Alpha data"
                            " but found RGB colors;"
                            " recompile with PNG_READ_RGB_TO_GRAY_SUPPORTED");
#endif
            }
            if (io.get_bit_depth() < 8)
            {
#ifdef PNG_READ_EXPAND_SUPPORTED
                io.set_gray_1_2_4_to_8();
#else
                throw error("convert_color_space: expected RGBA data"
                            " but found grayscale (< 8-bit) colors;"
                            " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
            }
        }

    private:
        uint_32 m_filler;
    };

} // namespace png

#endif // PNGPP_CONVERT_COLOR_SPACE_HPP_INCLUDED
