#ifndef PNGPP_GENERATOR_HPP_INCLUDED
#define PNGPP_GENERATOR_HPP_INCLUDED

#include <cassert>
#include "image_info.hpp"
#include "writer.hpp"

namespace png
{

    template< typename pixel, class pixgen >
    class generator
    {
    public:
        void write(std::ostream& stream)
        {
            writer wr(stream);
            wr.set_image_info(m_info);
            wr.write_info();

            size_t pass_count;
            if (m_info.get_interlace_type() != interlace_none)
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
            assert(pixel_gen); // TODO: can this be catched at
                               // comple/link time?
            for (size_t pass = 0; pass < pass_count; ++pass)
            {
                pixel_gen->reset(pass);

                for (size_t pos = 0; pos < m_info.get_height(); ++pos)
                {
                    wr.write_row(pixel_gen->get_next_row(pos));
                }
            }

            wr.write_end_info();
        }

    protected:
        explicit generator(image_info const& info)
            : m_info(info)
        {
        }

        generator(size_t width, size_t height)
            : m_info(make_image_info< pixel >())
        {
            m_info.set_width(width);
            m_info.set_height(height);
        }

        void reset(size_t /*pass*/)
        {
            // nothing to do in the most general case
        }

        png::image_info m_info;
    };

} // namespace png

#endif // PNGPP_GENERATOR_HPP_INCLUDED
