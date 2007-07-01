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
#ifndef PNGPP_GENERATOR_HPP_INCLUDED
#define PNGPP_GENERATOR_HPP_INCLUDED

#include <cassert>
#include "image_info.hpp"
#include "pixel_traits.hpp"
#include "writer.hpp"

namespace png
{

    template< typename pixel, class pixgen,
              class info_holder = def_image_info_holder >
    class generator
    {
    public:
        typedef pixel_traits< pixel > traits;

        void write(std::ostream& stream)
        {
            writer wr(stream);
            wr.set_image_info(get_info());
            wr.write_info();

            size_t pass_count;
            if (get_info().get_interlace_type() != interlace_none)
            {
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
                pass_count = wr.set_interlace_handling();
#else
                throw error("Cannot write interlaced image"
                            " -- interlace handling disabled.");
#endif
            }
            else
            {
                pass_count = 1;
            }
            pixgen* pixel_gen = static_cast< pixgen* >(this);
            assert(pixel_gen); // TODO: can this be caught at
                               // comple/link time?
            for (size_t pass = 0; pass < pass_count; ++pass)
            {
                pixel_gen->reset(pass);

                for (size_t pos = 0; pos < get_info().get_height(); ++pos)
                {
                    wr.write_row(pixel_gen->get_next_row(pos));
                }
            }

            wr.write_end_info();
        }

        image_info const& get_info() const
        {
            return m_info_holder.get_info();
        }

    protected:
        explicit generator(image_info& info)
            : m_info_holder(info)
        {
        }

        generator(size_t width, size_t height)
            : m_info_holder(make_image_info< pixel >())
        {
            get_info().set_width(width);
            get_info().set_height(height);
        }

        void reset(size_t /*pass*/)
        {
            // nothing to do in the most general case
        }

        image_info& get_info()
        {
            return m_info_holder.get_info();
        }

        info_holder m_info_holder;
    };

} // namespace png

#endif // PNGPP_GENERATOR_HPP_INCLUDED
