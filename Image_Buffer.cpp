//
// Created by JULIA BALAJAN on 2/04/2017.
//

#include "Image_Buffer.h"
#include <png.h>
#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <memory>

Image_Buffer::Image_Buffer(std::unique_ptr<Bounds2D<int>> &bnds, const std::string &src) : Buffer_Base(bnds), file_src(src) { }

void Image_Buffer::flush() {
    fp = fopen(file_src.c_str(), "wb");
    if (!fp) {
        std::ostringstream ss;
        ss << "error: Unable to open file " << file_src << " for writing";
        throw std::runtime_error(ss.str());
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr) {
        throw std::runtime_error("error: png_create_write_struct failed");
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        throw std::runtime_error("error: png_create_info_struct failed");
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        throw std::runtime_error("Error during init_io");
    }

    png_init_io(png_ptr, fp);

    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr, info_ptr, bounds->width(), bounds->height(),
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_text title_text;
    title_text.compression = PNG_TEXT_COMPRESSION_NONE;
    title_text.key = "Title";
    title_text.text = (char *)file_src.c_str();
    png_set_text(png_ptr, info_ptr, &title_text, 1);

    png_write_info(png_ptr, info_ptr);

    std::vector<RGB> row(3 * bounds->width());
    auto first = buffer.begin();
    auto last = buffer.begin() + bounds->width();

    while (first != buffer.end()) {
        std::copy(first, last, row.begin());
        png_write_row(png_ptr, (png_bytep)&row[0]);
        first = last;
        last += bounds->width();
    }



    png_write_end(png_ptr, NULL);

    png_init_io(png_ptr, fp);
}

Image_Buffer::~Image_Buffer() {
    if (fp) fclose(fp);
    if (info_ptr) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr) png_destroy_write_struct(&png_ptr, static_cast<png_infopp>(NULL));
}