#ifndef MANDELBROT_FRACTAL_DRAWER_WINDOW_H
#define MANDELBROT_FRACTAL_DRAWER_WINDOW_H

#include <complex>

template<typename T>
class Bounds2D {
    T x_min, x_max, y_min, y_max;
public:
    Bounds2D(T x_min, T x_max, T y_min, T y_max) : x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max) { }

// Util functions
    T width() const {
        return (x_max - x_min);
    }

    T height() const {
        return (y_max - y_min);
    }

    T size() const {
        return (height() * width());
    }

// Setters and getters
    T get_y_min() const {
        return y_min;
    }

    T get_y_max() const {
        return y_max;
    }

    T get_x_min() const {
        return x_min;
    }

    T get_x_max() const {
        return x_max;
    }

    void set_y_min(T _y_min) {
        this->y_min = _y_min;
    }

    void set_y_max(T _y_max) {
        this->y_max = _y_max;
    }

    void set_x_min(T _x_min) {
        this->x_min = _x_min;
    }

    void set_x_max(T _x_max) {
        this->x_max = _x_max;
    }

// Reset values
    void reset(T x_min, T x_max, T y_min, T y_max) {
        y_min(y_min);
        y_max(y_max);
        x_min(x_min);
        x_max(x_max);
    }
};


#endif //MANDELBROT_FRACTAL_DRAWER_WINDOW_H
