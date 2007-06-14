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
#include "gray_pixel.hpp"
#include "ga_pixel.hpp"

namespace png
{

    /**
     * \brief  IO transformation class template.  Converts image color
     * space.
     *
     * This IO transformation class template is used to convert color
     * space of the source image to color space of target image.  An
     * error with human-readable description is thrown when color
     * space could not be converted.  Often, this means that you have
     * to recompile libpng with some more conversion options turned
     * on.
     *
     * \see  image, image::read
     */
    template< typename pixel >
    struct convert_color_space
    {
        typedef pixel_traits< pixel > traits;

        // traits::component_type filler = traits::alpha_filler()
        explicit convert_color_space(uint_32 filler = 0xffff)
            : m_filler(filler)
        {
        }

        void operator()(reader& io) const
        {
            handle_16(io);
            handle_alpha(io, m_filler);
            handle_palette(io);
            handle_rgb(io);
            handle_gray(io);
        }

        void operator()(writer& io) const;

    protected:
        static void handle_16(reader& io)
        {
            if (io.get_bit_depth() == 16
                && traits::bit_depth == 8) // TODO: 1, 2, 4?
            {
#ifdef PNG_READ_16_TO_8_SUPPORTED
                io.set_strip_16();
#else
                throw error("expected 8-bit data but found 16-bit;"
                            " recompile with PNG_READ_16_TO_8_SUPPORTED");
#endif
            }
        }

        static void handle_alpha(reader& io, uint_32 filler)
        {
            bool src_alpha = io.get_color_type() & color_mask_alpha;
            bool dst_alpha = traits::color_space & color_mask_alpha;
            if (src_alpha && !dst_alpha)
            {
#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
                io.set_strip_alpha();
#else
                throw error("alpha channel unexpected;"
                            " recompile with PNG_READ_STRIP_ALPHA_SUPPORTED");
#endif
            }
            if (!src_alpha && dst_alpha)
            {
#if defined(PNG_tRNS_SUPPORTED) && defined(PNG_READ_EXPAND_SUPPORTED)
                if ((io.get_color_type() & color_mask_palette)
                    && io.has_chunk(chunk_tRNS))
                {
                    io.set_tRNS_to_alpha();
                    return;
                }
#endif
#if defined(PNG_READ_FILLER_SUPPORTED) && !defined(PNG_1_0_X)
                io.set_add_alpha(filler, filler_after);
#else
                throw error("expected alpha channel but none found;"
                            " recompile with PNG_READ_FILLER_SUPPORTED"
                            " and be sure to use libpng > 1.0.x");
#endif
            }
        }

        static void handle_palette(reader& io)
        {
            if (io.get_color_type() == color_type_palette)
            {
#ifdef PNG_READ_EXPAND_SUPPORTED
                io.set_palette_to_rgb();
#else
                throw error("indexed colors unexpected;"
                            " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
            }
        }

        static void handle_rgb(reader& io)
        {
            bool src_rgb =
                io.get_color_type() & (color_mask_rgb | color_mask_palette);
            bool dst_rgb = traits::color_space & color_mask_rgb;
            if (src_rgb && !dst_rgb)
            {
#ifdef PNG_READ_RGB_TO_GRAY_SUPPORTED
                io.set_rgb_to_gray();
#else
                throw error("grayscale data expected;"
                            " recompile with PNG_READ_RGB_TO_GRAY_SUPPORTED");
#endif
            }
            if (!src_rgb && dst_rgb)
            {
#ifdef PNG_READ_GRAY_TO_RGB_SUPPORTED
                io.set_gray_to_rgb();
#else
                throw error("expected RGB data;"
                            " recompile with PNG_READ_GRAY_TO_RGB_SUPPORTED");
#endif
            }
        }

        static void handle_gray(reader& io)
        {
            if ((io.get_color_type() & ~color_mask_alpha) == color_type_gray)
            {
                if (io.get_bit_depth() < 8)
                {
#ifdef PNG_READ_EXPAND_SUPPORTED
                    io.set_gray_1_2_4_to_8();
#else
                    throw error("convert_color_space: expected RGB data"
                                " but found grayscale (< 8-bit) colors;"
                                " recompile with PNG_READ_EXPAND_SUPPORTED");
#endif
                }
            }
        }

        uint_32 m_filler;
    };

} // namespace png

#endif // PNGPP_CONVERT_COLOR_SPACE_HPP_INCLUDED
