//
// Created by Balajanovski on 2/04/2017.
//

#ifndef MANDELBROT_FRACTAL_DRAWER_IMAGE_BUFFER_H
#define MANDELBROT_FRACTAL_DRAWER_IMAGE_BUFFER_H

#include <string>
#include "Pixel_Stream_Base.h"
#include "RGB.h"
#include <png.h>
#include <memory>

#define PNG_DEBUG 3

class Image_Stream : public Pixel_Stream_Base {
    // Location to write image to
    std::string file_src;

    // PNG data
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep row;


    // File pointer
    FILE *fp;
public:
    Image_Stream(std::shared_ptr<Bounds2D<int>> &, const std::string &);

    ~Image_Stream();

    virtual void flush() override;
};


#endif //MANDELBROT_FRACTAL_DRAWER_IMAGE_BUFFER_H
