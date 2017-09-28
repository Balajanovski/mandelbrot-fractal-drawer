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

    // Check for any errors in the building of the kernel
    try {
        program.build(devices);
    }
    catch (cl::Error& e) {
        if (e.err() == CL_BUILD_PROGRAM_FAILURE) {
            for (cl::Device dev : devices) {
                // Check the build status
                cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(dev);
                if (status != CL_BUILD_ERROR)
                    continue;

                // Get the build log
                std::string name = dev.getInfo<CL_DEVICE_NAME>();
                std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(dev);
                std::cerr << "Build log for " << name << ":" << std::endl
                          << buildlog << std::endl;
            }
        }
        else {
            throw e;
        }
    }

    kernel = std::move(cl::Kernel(program, "render"));
}

Pixel_Calculator::Pixel_Calculator(const char *kernel_src, const std::shared_ptr<Pixel_Stream_Base> &stream, RGB color, const Bounds2D<float> &plane) :
bound_stream(stream),
complex_plane(plane) {

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

    // Notify kernel of the colour of the fractal
    cl_color = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(color), (void*)&color);
    kernel.setArg(1, cl_color);

    // Create temporary array to notify the kernel of the complex plane's dimensions
    float plane_array[] = {complex_plane.get_x_min(), complex_plane.get_x_max(),
                         complex_plane.get_y_min(), complex_plane.get_y_max()};

    // Notify kernel of the complex plane in which the fractal will be drawn
    cl_plane = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                           sizeof(plane_array), (void*)&plane_array);
    kernel.setArg(2, cl_plane);
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