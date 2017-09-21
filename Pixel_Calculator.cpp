#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include <utility>
#include <cassert>

#include "Pixel_Calculator.h"

void Pixel_Calculator::compile_kernel(const char* src, const cl::Context& context, cl::Kernel& kernel) {
    std::ifstream is(src);
    std::string code;

    std::string temp_str;
    while (std::getline(is, temp_str)) {
        code += temp_str + '\n';
    }

    cl::Program::Sources source;
    source.push_back({code.c_str(), code.length()});

    cl::Program program(context, source);

    program.build();


    kernel = std::move(cl::Kernel(program, "render"));
}

Pixel_Calculator::Pixel_Calculator(const char *kernel_src, const std::shared_ptr<Pixel_Stream_Base> &stream, RGB color) :
bound_stream(stream) {

    cl::Platform platform;
    cl::Platform::get(&platform);


    cl_context_properties cps[3] = {
            CL_CONTEXT_PLATFORM,
            (cl_context_properties)(platform)(),
            0
    };
    context = cl::Context(CL_DEVICE_TYPE_GPU, cps);

    devices = context.getInfo<CL_CONTEXT_DEVICES>();

    // Create a command queue
    for (int i = 0; i < devices.size(); ++i)
        queues.push_back(cl::CommandQueue(context, devices[i]));

    // Create buffer to store result
    cl_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, (bound_stream->buffer.size() * sizeof(RGB)));

    compile_kernel(kernel_src, context, kernel);

    kernel.setArg(0, cl_buffer);

    // Create opencl buffer with previously created structure to notify the kernel what color the fractal
    // will be drawn in
    cl_color = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(color), (void*)&color);
    kernel.setArg(1, cl_color);
}

void Pixel_Calculator::calculate() {
    size_t num_devices = devices.size();
    size_t width = bound_stream->bounds->width();
    size_t height = bound_stream->bounds->height();

    assert(num_devices > 0);
    size_t device_work_size[2] = {width, height / num_devices};

    for(int i = 0; i < num_devices; ++i) {
        size_t device_work_offset[2] = {0, device_work_size[1] * i};
        size_t offset = device_work_offset[1] * sizeof(RGB) * width;
        queues[i].enqueueNDRangeKernel(kernel, cl::NDRange(device_work_offset[0], device_work_offset[1]),
                                   cl::NDRange(device_work_size[0], device_work_size[1]));

        // Non-blocking read, so we can continue queuing up more kernels
        queues[i].enqueueReadBuffer(cl_buffer, CL_FALSE, offset, (width * height * sizeof(RGB)) / num_devices, &bound_stream->buffer[0]);
    }

    for(int i = 0; i < num_devices; ++i) {
        queues[i].finish();
    }

}