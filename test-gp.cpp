#include <iostream>
#include <ostream>

#include "png.hpp"

void
print_usage()
{
    std::cerr << "usage: test-gp 1|2|4 OUTFILE" << std::endl;
}

template< class pixel >
void
generate_image(png::image< pixel >& image)
{
    for (size_t j = 0; j < image.get_height(); ++j)
    {
        for (size_t i = 0; i < image.get_width(); ++i)
        {
            image.set_pixel(i, j, pixel(i + j));
        }
    }
}

int
main(int argc, char* argv[])
try
{
    if (argc != 3)
    {
        print_usage();
        return EXIT_FAILURE;
    }
    char const* bits = argv[1];
    char const* filename = argv[2];

    size_t const width = 32;
    size_t const height = 32;

    if (strcmp(bits, "1") == 0)
    {
        png::image< png::gray_pixel_1 > image(width, height);
        generate_image(image);
        image.write(filename);
    }
    else if (strcmp(bits, "2") == 0)
    {
        png::image< png::gray_pixel_2 > image(width, height);
        generate_image(image);
        image.write(filename);
    }
    else if (strcmp(bits, "4") == 0)
    {
        png::image< png::gray_pixel_4 > image(width, height);
        generate_image(image);
        image.write(filename);
    }
    else
    {
        print_usage();
        return EXIT_FAILURE;
    }
}
catch (std::exception const& error)
{
    std::cerr << "test-gp: " << error.what() << std::endl;
    return EXIT_FAILURE;
}
