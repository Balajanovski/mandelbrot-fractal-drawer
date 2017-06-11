//
// Created by JULIA BALAJAN on 1/04/2017.
//

#ifndef MANDELBROT_FRACTAL_DRAWER_BUFFER_BASE_H
#define MANDELBROT_FRACTAL_DRAWER_BUFFER_BASE_H

#include "OpenCL/cl.hpp"
#include <memory>
#include <vector>

#include "Bounds2D.h"
#include "RGB.h"

class Pixel_Stream_Base {
friend class Pixel_Calculator;
    // Stores the pixel data
    std::vector<RGB> buffer;
protected:
    std::vector<RGB> &get_buffer() {
        return buffer;
    }

    // Iterator to where in the buffer the appending is happening
    typename std::vector<RGB>::iterator pos_iter;

    // Represents the size of the window to which the buffer is writing
    std::shared_ptr<Bounds2D<int>> bounds;
public:
    Pixel_Stream_Base(std::shared_ptr<Bounds2D<int>> &bnd) :
            buffer(bnd->size()), bounds(bnd) { pos_iter = buffer.begin(); }
    virtual ~Pixel_Stream_Base() { };
    virtual void flush() = 0;

    Pixel_Stream_Base &operator<<(RGB &&val) {
        if (pos_iter != buffer.end()) {
            *(pos_iter) = std::move(val);
            ++pos_iter;
        }
        return *this;
    }
};

#endif //MANDELBROT_FRACTAL_DRAWER_BUFFER_BASE_H
