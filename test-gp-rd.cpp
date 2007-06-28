#include <iostream>
#include <ostream>

#include "png.hpp"

void
print_usage()
{
    std::cerr << "usage: test-gp-rd 1|2|4 INFILE OUTFILE" << std::endl;
}

template< class pixel >
void
test_image(char const* infile, char const* outfile)
{
    png::image< pixel > image(infile, png::require_color_space< pixel >());
    image.write(outfile);
}

int
main(int argc, char* argv[])
try
{
    if (argc != 4)
    {
        print_usage();
        return EXIT_FAILURE;
    }
    char const* bits = argv[1];
    char const* infile = argv[2];
    char const* outfile = argv[3];

    if (strcmp(bits, "1") == 0)
    {
        test_image< png::gray_pixel_1 >(infile, outfile);
    }
    else if (strcmp(bits, "2") == 0)
    {
        test_image< png::gray_pixel_2 >(infile, outfile);
    }
    else if (strcmp(bits, "4") == 0)
    {
        test_image< png::gray_pixel_4 >(infile, outfile);
    }
    else
    {
        print_usage();
        return EXIT_FAILURE;
    }
}
catch (std::exception const& error)
{
    std::cerr << "test-gp-rd: " << error.what() << std::endl;
    return EXIT_FAILURE;
}
