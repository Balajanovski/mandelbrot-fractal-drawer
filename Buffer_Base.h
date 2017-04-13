//
// Created by JULIA BALAJAN on 1/04/2017.
//

#ifndef MANDELBROT_FRACTAL_DRAWER_BUFFER_BASE_H
#define MANDELBROT_FRACTAL_DRAWER_BUFFER_BASE_H

#include <vector>
#include <memory>

#include "Bounds2D.h"

template <typename T>
class Buffer_Base {
protected:
    // The buffer itself
    std::vector<T> buffer;

    // Iterator to where in the buffer the appending is happening
    typename std::vector<T>::iterator pos_iter;

    // Represents the size of the window to which the buffer is writing
    std::unique_ptr<Bounds2D<int>> bounds;
public:
    Buffer_Base(std::unique_ptr<Bounds2D<int>> &bnd) :
            buffer(bnd->size()), bounds(std::move(bnd)) { pos_iter = buffer.begin(); }
    virtual ~Buffer_Base() { };
    virtual void flush() = 0;

    Buffer_Base<T> &operator<<(T &&val) {
        if (pos_iter != buffer.end()) {
            *(pos_iter) = std::move(val);
            ++pos_iter;
        }
        return *this;
    }
};

#endif //MANDELBROT_FRACTAL_DRAWER_BUFFER_BASE_H
