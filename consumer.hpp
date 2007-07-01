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
#ifndef PNGPP_CONSUMER_HPP_INCLUDED
#define PNGPP_CONSUMER_HPP_INCLUDED

#include <cassert>
#include "streaming_base.hpp"
#include "reader.hpp"

namespace png
{

    template< typename pixel, class pixcon,
              class info_holder = def_image_info_holder >
    class consumer
        : public streaming_base< pixel, info_holder >
    {
    public:
        typedef pixel_traits< pixel > traits;

        template< class transformation >
        void read(std::istream& stream, transformation const& transform)
        {
            reader rd(stream);
            rd.read_info();
            transform(rd);

            // interlace handling _must_ be set up prior to info update
            size_t pass_count;
            if (rd.get_interlace_type() != interlace_none)
            {
#ifdef PNG_READ_INTERLACING_SUPPORTED
                pass_count = rd.set_interlace_handling();
#else
                throw error("Cannot read interlaced image"
                            " -- interlace handling disabled.");
#endif
            }
            else
            {
                pass_count = 1;
            }

            rd.update_info();
            if (rd.get_color_type() != traits::color_space
                || rd.get_bit_depth() != traits::bit_depth)
            {
                throw std::logic_error("color type and/or bit depth mismatch"
                                       " in png::consumer::read()");
            }

            this->get_info() = rd.get_image_info();

            pixcon* pixel_con = static_cast< pixcon* >(this);
            assert(pixel_con); // TODO: can this be caught at
                               // comple/link time?
            for (size_t pass = 0; pass < pass_count; ++pass)
            {
                pixel_con->reset(pass);

                for (size_t pos = 0; pos < this->get_info().get_height(); ++pos)
                {
                    rd.read_row(pixel_con->get_next_row(pos));
                }
            }

            rd.read_end_info();
        }

    protected:
        typedef streaming_base< pixel, info_holder > base;

        explicit consumer(image_info& info)
            : base(info)
        {
        }

        consumer(size_t width, size_t height)
            : base(width, height)
        {
        }
    };

} // namespace png

#endif // PNGPP_CONSUMER_HPP_INCLUDED
