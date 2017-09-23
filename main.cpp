#define __CL_ENABLE_EXCEPTIONS

#include <complex>
#include <iostream>
#include <memory>
#include <regex>

#include "Bounds2D.h"
#include "Screen_Stream.h"
#include "Image_Stream.h"
#include "Pixel_Stream_Base.h"
#include "Pixel_Calculator.h"

RGB retrieve_color(const std::regex&);

int main() {
    std::cout << "Running mandelbrot-fractal-drawer by Balajanovski..." << std::endl;

    // Declare window object to represent the complex plane
    //Bounds2D<float> complex_plane(-2.2, 1.2, -1.7, 1.7);
    Bounds2D<float> complex_plane(-2.2, 1.2, -1.7, 1.7);

    // Declare window object to represent the OpenGL window
    auto window = std::make_shared<Bounds2D<int>>(0, ((std::abs(complex_plane.get_x_min()) + complex_plane.get_x_max()) / COMPLEX_INCREMENT),
                       0, ((std::abs(complex_plane.get_y_min()) + complex_plane.get_y_max()) / COMPLEX_INCREMENT));

    std::shared_ptr<Pixel_Stream_Base> pixel_buffer;

    // Get the fractal colour
    std::regex pattern("\\{[ ]*([0-9]{1,3})[ ]*,[ ]*([0-9]{1,3})[ ]*,[ ]*([0-9]{1,3})[ ]*\\}");
    RGB chosen_color;
    bool input_correct_rgb = false;
    while (!input_correct_rgb) {
        input_correct_rgb = true;
        std::cout << "\nPlease enter the color you want the fractal to be drawn in RGB format: {R,G,B}.\nNumbers may be from 0 to 255" << std::endl;

        try {
            chosen_color = retrieve_color(pattern);
        }
        catch (std::runtime_error e) {
            std::cout << e.what() << std::endl;
            input_correct_rgb = false;
        }
    }

    std::cout << "\nWould you like to draw fractal to a window or an image?\n"
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





    Pixel_Calculator pixel_calculator("accelerated_calculation.cl", pixel_buffer, chosen_color);
    pixel_calculator.calculate();

    pixel_buffer->flush();


    std::cout << "\nClosing down..." << std::endl;

}

RGB retrieve_color(const std::regex& pattern) {
        std::string input;

        std::getline(std::cin, input);

        std::smatch regex_results;

        RGB processed_results;
        if (std::regex_match(input, regex_results, pattern)) {

            // Make sure input values are not greater than 255
            if (std::stoi(regex_results[1].str()) > 255 ||
                std::stoi(regex_results[2].str()) > 255 ||
                std::stoi(regex_results[3].str()) > 255)
            {
                throw std::runtime_error("error: RGB values must be between 0 and 255");
            }

            processed_results.r = static_cast<uint8_t>(std::stoi(regex_results[1].str()));
            processed_results.g = static_cast<uint8_t>(std::stoi(regex_results[2].str()));
            processed_results.b = static_cast<uint8_t>(std::stoi(regex_results[3].str()));
        }
        else {
            throw std::runtime_error("error: invalid RGB sequence input");
        }

        return processed_results;
}