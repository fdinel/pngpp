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
#ifndef PNGPP_INFO_HPP_INCLUDED
#define PNGPP_INFO_HPP_INCLUDED

#include <cassert>
#include "info_base.hpp"

namespace png
{

    /**
     * \brief  Holds information about PNG image.
     */
    class info
        : public info_base
    {
    public:
        explicit info(png_struct* png)
            : info_base(png),
              m_width(0),
              m_height(0),
              m_bit_depth(0),
              m_color_type(color_type_none),
              m_interlace(interlace_none),
              m_compression_type(compression_type_default),
              m_filter_type(filter_type_default)
        {
        }

        void read()
        {
            assert(m_png);
            assert(m_info);

            png_read_info(m_png, m_info);
            png_get_IHDR(m_png,
                         m_info,
                         & m_width,
                         & m_height,
                         & m_bit_depth,
                         reinterpret_cast< int* >(& m_color_type),
                         reinterpret_cast< int* >(& m_interlace),
                         reinterpret_cast< int* >(& m_compression_type),
                         reinterpret_cast< int* >(& m_filter_type));
        }

        void write() const
        {
            assert(m_png);
            assert(m_info);

            sync_ihdr();
            png_write_info(m_png, m_info);
        }

        void update()
        {
            assert(m_png);
            assert(m_info);

            sync_ihdr();
            png_read_update_info(m_png, m_info);
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
            return m_interlace;
        }

        void set_interlace_type(interlace_type interlace)
        {
            m_interlace = interlace;
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

    protected:
        void sync_ihdr(void) const
        {
            png_set_IHDR(m_png,
                         m_info,
                         m_width,
                         m_height,
                         m_bit_depth,
                         m_color_type,
                         m_interlace,
                         m_compression_type,
                         m_filter_type);
        }

        uint_32 m_width;
        uint_32 m_height;
        int m_bit_depth;
        color_type m_color_type;
        interlace_type m_interlace;
        compression_type m_compression_type;
        filter_type m_filter_type;
    };

} // namespace png

#endif // PNGPP_INFO_HPP_INCLUDED
