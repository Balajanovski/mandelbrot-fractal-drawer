#ifndef MANDELBROT_FRACTAL_DRAWER_PIXEL_CALCULATOR_H
#define MANDELBROT_FRACTAL_DRAWER_PIXEL_CALCULATOR_H

#define __CL_ENABLE_EXCEPTIONS

#include "OpenCL/cl.hpp"
#include "Pixel_Stream_Base.h"

#include <memory>
#include <vector>

static constexpr float COMPLEX_INCREMENT = 0.005f;

class Pixel_Calculator {
    // Stream to which the pixel calculator is bound
    std::shared_ptr<Pixel_Stream_Base> bound_stream;

    cl::Context context;

    void compile_kernel(const char* src, const cl::Context& context, cl::Kernel& kernel);
    cl::Buffer cl_buffer;

    cl::Buffer cl_color;

    std::vector<cl::Device> devices;

    std::vector<cl::CommandQueue> queues;

    cl::Kernel kernel;
public:
    Pixel_Calculator(const char *, const std::shared_ptr<Pixel_Stream_Base> &, const RGB);

    void calculate();
};


#endif //MANDELBROT_FRACTAL_DRAWER_PIXEL_CALCULATOR_H
