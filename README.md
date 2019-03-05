Braille
=======

Braille converts any image into a black-and-white text representation using the [Braille Patterns](https://en.wikipedia.org/wiki/Braille_Patterns) in the unicode standard.


### Requirements

- A C++11 compiler
- OpenCV 4

### Usage

`braille <filename> [-l|--max-length N] [-t|--threshold N]`


`--max-length, -l`

The maximum amount of characters in the output. Must be larger than 0.

Defaults to the maximum value of the `int` type.

`--threshold, -t`

The image is loaded as an 8-bit greyscale image. The threshold is the lower value bound for the pixel to be converted into black. Must be in the range [0, 256]

Defaults to 128.
