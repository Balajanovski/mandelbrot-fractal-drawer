//
// Created by JULIA BALAJAN on 18/03/2017.
//

#ifndef MANDELBROT_FRACTAL_DRAWER_WINDOW_H
#define MANDELBROT_FRACTAL_DRAWER_WINDOW_H

#include <complex>

template<typename T>
class Window {
    T _x_min, _x_max, _y_min, _y_max;
public:
    Window(T x_min, T x_max, T y_min, T y_max) : _x_min(x_min), _x_max(x_max), _y_min(y_min), _y_max(y_max) { }

// Util functions
    T width() const {
        return (_x_max - _x_min);
    }

    T height() const {
        return (_y_max - _y_min);
    }

    T size() const {
        return (height() * width());
    }

// Setters and getters
    T get_y_min() const {
        return _y_min;
    }

    T get_y_max() const {
        return _y_max;
    }

    T get_x_min() const {
        return _x_min;
    }

    T get_x_max() const {
        return _x_max;
    }

    void set_y_min(T _y_min) {
        Window::_y_min = _y_min;
    }

    void set_y_max(T _y_max) {
        Window::_y_max = _y_max;
    }

    void set_x_min(T _x_min) {
        Window::_x_min = _x_min;
    }

    void set_x_max(T _x_max) {
        Window::_x_max = _x_max;
    }

// Reset values
    void reset(T x_min, T x_max, T y_min, T y_max) {
        _y_min(y_min);
        _y_max(y_max);
        _x_min(x_min);
        _x_max(x_max);
    }
};


#endif //MANDELBROT_FRACTAL_DRAWER_WINDOW_H
