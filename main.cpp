#include <complex>
#include <iostream>
#include <stdint.h>
#include "Window.h"
#include "Draw_Buffer.h"

static constexpr float COMPLEX_INCREMENT = 0.01f;
static constexpr int MULTIPLICATION_FACTOR = 1000;

template <typename T>
void mandelbrot(std::complex<T> &z, const std::complex<T> &c) {
    z = (z * z) + c;
}

template <unsigned factor, typename T>
RGB calculate_pixel(const std::complex<T> &c) {
    std::complex<T> z(0, 0);
    unsigned int counter = 0;

    while (counter++ < 256 && !(std::abs(z) > 2 * factor)) {
        mandelbrot(z, c);
    }

    if (counter < 256 && !(std::abs(z) > 2 * factor)) {
        return RGB{0, 0, 0};
    }

    else {
        float blue = 100 + counter % 155;
        return RGB{94, 219, blue};
    }
}

int main() {
    // Declare window object to represent the complex plane
    Window<int64_t> complex_plane(-2.0 * MULTIPLICATION_FACTOR, 1.0 * MULTIPLICATION_FACTOR, -1.0 * MULTIPLICATION_FACTOR, 1.0 * MULTIPLICATION_FACTOR);

    // Declare window object to represent the OpenGL window
    Window<int> gl_window(0, (std::abs(complex_plane.get_x_min()) + complex_plane.get_x_max()) / (COMPLEX_INCREMENT * MULTIPLICATION_FACTOR),
                       0, (std::abs(complex_plane.get_y_min()) + complex_plane.get_y_max()) / (COMPLEX_INCREMENT * MULTIPLICATION_FACTOR));

    // Buffer pixels to this object
    Draw_Buffer pixel_buffer(&gl_window, "vertex_shader.glsl", "fragment_shader.glsl");

    // Iterate through the complex plane, finding out what colour the pixels are
    std::complex<int64_t> pixel_iterator(complex_plane.get_x_min(), complex_plane.get_y_max());
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