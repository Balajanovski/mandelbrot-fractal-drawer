#define __CL_ENABLE_EXCEPTIONS

#include <complex>
#include <iostream>
#include <memory>
#include <regex>
#include <tuple>

#include "Bounds2D.h"
#include "Screen_Stream.h"
#include "Image_Stream.h"
#include "Pixel_Stream_Base.h"
#include "Pixel_Calculator.h"

RGB retrieve_color(const std::regex&);

std::tuple<float,
           float,
           float,
           float> retrieve_draw_pos(const std::regex&);

int main() {
    std::cout << "Running mandelbrot-fractal-drawer by Balajanovski..." << std::endl;

    // Use regex to get the user's desired fractal colour
    std::regex retrieve_RGB_pattern("\\{ *([0-9]{1,3}) *, *([0-9]{1,3}) *, *([0-9]{1,3}) *\\}");
    RGB chosen_color;
    while (true) {
        std::cout << "\nPlease enter the color you want the fractal to be drawn in RGB format: {R,G,B}.\nNumbers may be from 0 to 255" << std::endl;

        try {
            chosen_color = retrieve_color(retrieve_RGB_pattern);
        }

        // If the input is invalid ask the user to input again
        catch (std::runtime_error e) {
            std::cout << e.what() << std::endl;
            continue;
        }

        break;
    }



    // Use regex to retrieve the user's desired complex plane
    // If input is invalid ask the user to
    std::regex retrieve_complex_pos_pattern("\\([ ]*([0-9.-]+) *,[ ]*([0-9.-]+) *,[ ]*([0-9.-]+) *,[ ]*([0-9.-]+) *\\)");
    std::tuple<float, float, float, float> chosen_range;
    while (true) {
        std::cout << "\nPlease enter the range of complex numbers within which you want the fractal to " <<
                  "be drawn. Suggested input: (-2, 3.25, -1.25, 2.5)" << std::endl;
        try {
            chosen_range = retrieve_draw_pos(retrieve_complex_pos_pattern);
        }

        // If the input is invalid ask the user to input again
        catch (std::runtime_error e) {
            std::cout << e.what() << std::endl;
            continue;
        }

        break;
    }



    // Declare window object to represent the complex plane with user's input
    Bounds2D<float> complex_plane(std::get<0>(chosen_range), std::get<1>(chosen_range),
                                  std::get<2>(chosen_range), std::get<3>(chosen_range));

    // Declare window object to represent the OpenGL window
    auto window = std::make_shared<Bounds2D<int>>(0, 1000, 0, 1000);

    std::cout << "\nWould you like to draw fractal to a window or an image?\n"
              << "Type W for window or I for image" << std::endl;

    // Create a pointer to a generic pixel buffer
    // To exploit polymorphic behavior
    std::shared_ptr<Pixel_Stream_Base> pixel_buffer;

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

    // Create a pixel calculator to use the GPU to accelerate the calculation
    Pixel_Calculator pixel_calculator("accelerated_calculation.cl", pixel_buffer, chosen_color, complex_plane);
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

std::tuple<float,
           float,
           float,
           float> retrieve_draw_pos(const std::regex& pattern) {
    std::string input;
    std::getline(std::cin, input);

    // Use regex to retrieve the draw pos
    std::smatch regex_results;
    std::tuple<float, float, float, float> processed_results;
    if (std::regex_match(input, regex_results, pattern)) {
        processed_results =
                std::make_tuple(
                        std::stof(regex_results[1].str()),
                        std::stof(regex_results[2].str()),
                        std::stof(regex_results[3].str()),
                        std::stof(regex_results[4].str())
                );
    }
    else {
        throw std::runtime_error("error: invalid complex plane input");
    }

    return processed_results;
}