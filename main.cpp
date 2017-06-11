//#define __CL_ENABLE_EXCEPTIONS

#include <complex>
#include <iostream>
#include <memory>

#include "Bounds2D.h"
#include "Screen_Stream.h"
#include "Image_Stream.h"
#include "Pixel_Stream_Base.h"
#include "Pixel_Calculator.h"

template <typename T>
int iterations_till_escape(const std::complex<T> &c, int max_iterations) {
    std::complex<T> z(0, 0);
    for (int iter = 0; iter < max_iterations; ++iter) {
        z = (z * z) + c;
        if (std::abs(z) > 2) {
            return iter;
        }
    }
    return -1;
}

template <typename T>
RGB calculate_pixel(const std::complex<T> &c) {
    int iterations = iterations_till_escape(c, 255);

    if (iterations == -1) {
        return RGB{0, 0, 0};
    }

    else {
        GLubyte blue = iterations * 5;
        return RGB{0, 0, blue};
    }
}

int main() {
    // Declare window object to represent the complex plane
    Bounds2D<float> complex_plane(-2.2, 1.2, -1.7, 1.7);

    // Declare window object to represent the OpenGL window
    auto window = std::make_shared<Bounds2D<int>>(0, ((std::abs(complex_plane.get_x_min()) + complex_plane.get_x_max()) / COMPLEX_INCREMENT),
                       0, ((std::abs(complex_plane.get_y_min()) + complex_plane.get_y_max()) / COMPLEX_INCREMENT));

    std::shared_ptr<Pixel_Stream_Base> pixel_buffer;


    std::cout << "Running mandelbrot-fractal-drawer...\nWould you like to draw fractal to a window or an image?\n"
              << "Type W for window or I for image" << std::endl;

    char response;
    while (!(std::cin >> response))
        ;
    if (response == 'W' || response == 'w') {
        // Initialise pointer to a draw buffer
        pixel_buffer.reset(new Screen_Stream(window, "vertex_shader.glsl", "fragment_shader.glsl"));
    }

    else if (response == 'I' || response == 'i') {
        std::cout << "\nPlease enter the location to where you want the fractal to be drawn" << std::endl;

        std::string src;
        while (!(std::cin >> src))
            ;

        // Initialise pointer to an image buffer
        pixel_buffer.reset(new Image_Stream(window, src));
    }

    else {
        std::cout << "\nInvalid response: Shutting down" << std::endl;
        exit(EXIT_FAILURE);
    }

    Pixel_Calculator pixel_calculator("accelerated_calculation.cl", pixel_buffer);
    pixel_calculator.calculate();

    pixel_buffer->flush();


    std::cout << "Closing down..." << std::endl;

}