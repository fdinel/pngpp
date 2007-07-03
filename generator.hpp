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
#include <stdexcept>
#include "error.hpp"
#include "streaming_base.hpp"
#include "writer.hpp"

namespace png
{

    template< typename pixel,
              class pixgen,
              class info_holder = def_image_info_holder,
              bool interlacing_supported = false >
    class generator
        : public streaming_base< pixel, info_holder >
    {
    public:
        void write(std::ostream& stream)
        {
            writer wr(stream);
            wr.set_image_info(this->get_info());
            wr.write_info();

            size_t pass_count;
            if (this->get_info().get_interlace_type() != interlace_none)
            {
#ifdef PNG_WRITE_INTERLACING_SUPPORTED
                if (interlacing_supported)
                {
                    pass_count = wr.set_interlace_handling();
                }
                else
                {
                    throw std::logic_error("Cannot write interlaced image --"
                                           " generator does not support it.");
                }
#else
                throw error("Cannot write interlaced image --"
                            " interlace handling disabled.");
#endif
            }
            else
            {
                pass_count = 1;
            }
            pixgen* pixel_gen = static_cast< pixgen* >(this);
            for (size_t pass = 0; pass < pass_count; ++pass)
            {
                pixel_gen->reset(pass);

                for (size_t pos = 0; pos < this->get_info().get_height(); ++pos)
                {
                    wr.write_row(pixel_gen->get_next_row(pos));
                }
            }

            wr.write_end_info();
        }

    protected:
        typedef streaming_base< pixel, info_holder > base;

        explicit generator(image_info& info)
            : base(info)
        {
        }

        generator(size_t width, size_t height)
            : base(width, height)
        {
        }
    };

} // namespace png

#endif // PNGPP_GENERATOR_HPP_INCLUDED
