#include <iostream>
#include <ostream>

#include <cstdlib>

#include "png.hpp"

class generator
{
public:
    explicit generator(size_t width, size_t height)
        : m_row(width),
          m_width(width),
          m_height(height),
          m_pos(0)
    {
        for (size_t i = 0; i <  m_row.size(); ++i)
        {
            m_row[i] = i > m_row.size() / 2;
        }
    }

    png::byte* next()
    {
        ++m_pos;

        size_t i = std::rand() % m_row.size();
        size_t j = std::rand() % m_row.size();
        png::gray_pixel_1 t = m_row[i];
        m_row[i] = m_row[j];
        m_row[j] = t;
        return reinterpret_cast< png::byte* >(row_traits::get_data(m_row));
    }

    void reset(int /*pass*/)
    {
        m_pos = 0;
    }

    bool end()
    {
        return m_pos == m_height;
    }

private:
typedef png::packed_pixel_row< png::gray_pixel_1 > row;
	typedef png::row_traits< row > row_traits;
	row m_row;
    size_t m_width;
    size_t m_height;
    size_t m_pos;
};

int
main()
try
{
    size_t const width = 32;
    size_t const height = 1024;

    std::ofstream file("output.png", std::ios::binary);

    png::writer writer(file);
    writer.set_width(width);
    writer.set_height(height);
    writer.set_color_type(png::color_type_gray);
    writer.set_bit_depth(1);
    writer.write_info();

    generator gen(width, height);
    writer.write_pixels(gen);

    writer.write_end_info();
}
catch (std::exception const& error)
{
    std::cerr << "example-generator: " << error.what() << std::endl;
    return EXIT_FAILURE;
}
