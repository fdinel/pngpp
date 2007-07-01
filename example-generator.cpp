#include <iostream>
#include <ostream>

#include <cstdlib>

#include "png.hpp"

class pixel_generator
    : public png::generator< png::gray_pixel_1, pixel_generator >
{
public:
    pixel_generator(size_t width, size_t height)
        : png::generator< png::gray_pixel_1, pixel_generator >(width, height),
          m_row(width)
    {
        for (size_t i = 0; i < m_row.size(); ++i)
        {
            m_row[i] = i > m_row.size() / 2 ? 1 : 0;
        }
    }

    png::byte* get_next_row(size_t /*pos*/)
    {
        size_t i = std::rand() % m_row.size();
        size_t j = std::rand() % m_row.size();
        png::gray_pixel_1 t = m_row[i];
        m_row[i] = m_row[j];
        m_row[j] = t;
        return reinterpret_cast< png::byte* >(row_traits::get_data(m_row));
    }

private:
    typedef png::packed_pixel_row< png::gray_pixel_1 > row;
	typedef png::row_traits< row > row_traits;
	row m_row;
};

int
main()
try
{
    size_t const width = 32;
    size_t const height = 512;

    std::ofstream file("output.png", std::ios::binary);
    pixel_generator generator(width, height);
    generator.write(file);
}
catch (std::exception const& error)
{
    std::cerr << "example-generator: " << error.what() << std::endl;
    return EXIT_FAILURE;
}
