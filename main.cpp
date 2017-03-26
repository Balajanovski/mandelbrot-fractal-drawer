#include <complex>
#include <iostream>
#include <stdint.h>
#include "Window.h"
#include "Draw_Buffer.h"

static constexpr float COMPLEX_INCREMENT = 0.005f;
static constexpr int MULTIPLICATION_FACTOR = 1;

template <unsigned factor, typename T>
int iterations_till_escape(const std::complex<T> &c, int max_iterations) {
    std::complex<T> z(0, 0);
    for (int iter = 0; iter < max_iterations; ++iter) {
        z = (z * z) + c;
        if (std::abs(z) > 2 * factor) {
            return iter;
        }
    }
    return -1;
}

template <unsigned factor, typename T>
RGB calculate_pixel(const std::complex<T> &c) {
    int iterations = iterations_till_escape<factor>(c, 255);

    if (iterations == -1) {
        return RGB{0, 0, 0};
    }

    else {
        GLbyte blue = iterations * 8;
        return RGB{0, 0, blue};
    }
}

int main() {
    // Declare window object to represent the complex plane
    Window<double> complex_plane(-2.2 * MULTIPLICATION_FACTOR, 1.2 * MULTIPLICATION_FACTOR, -1.7 * MULTIPLICATION_FACTOR, 1.7 * MULTIPLICATION_FACTOR);

    // Declare window object to represent the OpenGL window
    Window<int> gl_window(0, (std::abs(complex_plane.get_x_min()) + complex_plane.get_x_max()) / (COMPLEX_INCREMENT * MULTIPLICATION_FACTOR),
                       0, (std::abs(complex_plane.get_y_min()) + complex_plane.get_y_max()) / (COMPLEX_INCREMENT * MULTIPLICATION_FACTOR));

    // Buffer pixels to this object
    Draw_Buffer pixel_buffer(&gl_window, "vertex_shader.glsl", "fragment_shader.glsl");

    // Iterate through the complex plane, finding out what colour the pixels are
    std::complex<double> pixel_iterator(complex_plane.get_x_min(), complex_plane.get_y_max());
    while (pixel_iterator.imag() > complex_plane.get_y_min()) {
        while (pixel_iterator.real() < complex_plane.get_x_max()) {

            // Calculate the colour of the pixel using the mandelbrot function
            pixel_buffer << calculate_pixel<MULTIPLICATION_FACTOR>(pixel_iterator);

            // Increment
            pixel_iterator.real(pixel_iterator.real() + (COMPLEX_INCREMENT * MULTIPLICATION_FACTOR));
        }

        // Increment
        pixel_iterator.imag(pixel_iterator.imag() - (COMPLEX_INCREMENT * MULTIPLICATION_FACTOR));

        // Reset real iterator
        pixel_iterator.real(complex_plane.get_x_min());
    }

    pixel_buffer.flush();

    pixel_buffer.keep_window_open();

}