#ifndef MANDELBROT_FRACTAL_DRAWER_CLEANUP_H
#define MANDELBROT_FRACTAL_DRAWER_CLEANUP_H

#include <iostream>
#include <GLFW/glfw3.h>
#include "Get_GL.h"

template <class T>
void cleanup(T *) {
    static_assert(!sizeof(T*), "No overload of `cleanup` for this type.");
}

// Function to clean up glfw objects
template<typename... Args>
void cleanup(Args *... args){
    using ex = int[];
    (void)ex{(cleanup(args), 0)..., 0};
}

inline void cleanup(GLFWwindow *screen) {
    std::cout << "ting ling" << std::endl;
    glfwDestroyWindow(screen);
}

// Cleaner functor to clean up object
struct Cleaner {
    template<typename T>
    void operator() (T *p) const {
        cleanup(p);
    }

    template<typename T>
    void operator() (T p) const {
        cleanup(&p);
    }
};


#endif //MANDELBROT_FRACTAL_DRAWER_CLEANUP_H
