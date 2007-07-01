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
#ifndef PNGPP_IMAGE_INFO_HPP_INCLUDED
#define PNGPP_IMAGE_INFO_HPP_INCLUDED

#include "types.hpp"
#include "palette.hpp"

namespace png
{

    class image_info
    {
    public:
        image_info()
            : m_width(0),
              m_height(0),
              m_bit_depth(0),
              m_color_type(color_type_none),
              m_interlace_type(interlace_none),
              m_compression_type(compression_type_default),
              m_filter_type(filter_type_default)
        {
        }

        size_t get_width() const
        {
            return m_width;
        }

        void set_width(size_t width)
        {
            m_width = width;
        }

        size_t get_height() const
        {
            return m_height;
        }

        void set_height(size_t height)
        {
            m_height = height;
        }

        color_type get_color_type() const
        {
            return m_color_type;
        }

        void set_color_type(color_type color_space)
        {
            m_color_type = color_space;
        }

        int get_bit_depth() const
        {
            return m_bit_depth;
        }

        void set_bit_depth(int bit_depth)
        {
            m_bit_depth = bit_depth;
        }

        interlace_type get_interlace_type() const
        {
            return m_interlace_type;
        }

        void set_interlace_type(interlace_type interlace)
        {
            m_interlace_type = interlace;
        }

        compression_type get_compression_type() const
        {
            return m_compression_type;
        }

        void set_compression_type(compression_type compression)
        {
            m_compression_type = compression;
        }

        filter_type get_filter_type() const
        {
            return m_filter_type;
        }

        void set_filter_type(filter_type filter)
        {
            m_filter_type = filter;
        }

        palette const& get_palette() const
        {
            return m_palette;
        }

        palette& get_palette()
        {
            return m_palette;
        }

        void set_palette(palette const& plte)
        {
            m_palette = plte;
        }

        void drop_palette()
        {
            m_palette.clear();
        }

    protected:
        uint_32 m_width;
        uint_32 m_height;
        int m_bit_depth;
        color_type m_color_type;
        interlace_type m_interlace_type;
        compression_type m_compression_type;
        filter_type m_filter_type;
        palette m_palette;
    };

    class def_image_info_holder
    {
    public:
        def_image_info_holder(image_info const& info)
            : m_info(info)
        {
        }

        image_info& get_info()
        {
            return m_info;
        }

    private:
        image_info m_info;
    };

    class image_info_ref_holder
    {
    public:
        image_info_ref_holder(image_info& info)
            : m_info(info)
        {
        }

        image_info& get_info()
        {
            return m_info;
        }

    private:
        image_info& m_info;
    };

    template< typename pixel >
    image_info
    make_image_info()
    {
        typedef pixel_traits< pixel > traits;
        image_info info;
        info.set_color_type(traits::color_space);
        info.set_bit_depth(traits::bit_depth);
        return info;
    }

} // namespace png

#endif // PNGPP_IMAGE_INFO_HPP_INCLUDED
