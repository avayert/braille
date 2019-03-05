#include <algorithm>
#include <iostream>
#include <codecvt>
#include <locale>
#include <vector>
#include <cmath>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "CLI11.hpp"


// Hoooooo boy https://en.wikipedia.org/wiki/Braille_Patterns#Block
constexpr unsigned char offsets[4][2] = {
    {1,  8},
    {2,  16},
    {4,  32},
    {64, 128}
};
constexpr int unicode_start = 0x2800;


int main(int argc, char **argv) {
    CLI::App parser("Convert an image to braille.");

    std::string filename;
    parser.add_option("filename", filename, "filename")
        ->required()
        ->check(CLI::ExistingFile);

    int max_length = std::numeric_limits<int>::max();
    parser.add_option("-l, --max-length", max_length, "Maximum length of output.")
        ->check(CLI::Range(1, std::numeric_limits<int>::max()));

    int threshold = 128;
    parser.add_option("-t, --threshold", threshold, "Threshold to use for image.")
        ->check(CLI::Range(0, 256));

    CLI11_PARSE(parser, argc, argv);


    cv::Mat image = cv::imread(filename, cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cerr << "Could not load image: \"" << filename << "\"." << std::endl;
        return EXIT_FAILURE;
    }

    int width = image.cols;
    int height = image.rows;

    if ((width + 1) * height / 8 > max_length) {
        double ratio = width / static_cast<double>(height);
        width = static_cast<int>(std::sqrt(8 * max_length * ratio + 1) - 1);
        height = static_cast<int>(width / ratio);

        width -= width % 2;
        height -= height % 4;

        int one = 1;
        height = std::max(one, height);
        width = std::max(one, width);

        cv::Mat out(cv::Size{width, height}, image.depth());
        cv::resize(image, out, {width, height}, 0, 0);

        image = out;
    }

    // threshold image
    image = image >= threshold;


    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;

    for (int offset_y = 0; offset_y < height; offset_y += 4) {
        for (int offset_x = 0; offset_x < width; offset_x += 2) {
            char16_t c = unicode_start;

            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 2; x++) {
                    if (offset_y + y > (height - 1) || offset_x + x > (width - 1)) {
                        continue;
                    }

                    if (!image.at<bool>(offset_y + y, offset_x + x)) {
                        c += offsets[y][x];
                    }
                }

            }
            std::cout << converter.to_bytes(c);
        }
        std::cout << '\n';
    }

    return EXIT_SUCCESS;
}
