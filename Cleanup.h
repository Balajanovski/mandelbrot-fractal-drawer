#ifndef MANDELBROT_FRACTAL_DRAWER_CLEANUP_H
#define MANDELBROT_FRACTAL_DRAWER_CLEANUP_H

#include <GLFW/glfw3.h>
#include "Get_GL.h"

// Function to clean up glfw objects
template<typename... Args>
void cleanup(Args *... args){
    using ex = int[];
    (void)ex{(cleanup(args), 0)..., 0};
}

inline void cleanup(GLFWwindow *screen) {
    if (!screen)
        return;
    glfwDestroyWindow(screen);
}

// Cleaner functor to clean up object
struct Cleaner {
    template<typename T>
    void operator() (T *p) const {
        cleanup(p);
    }
};


#endif //MANDELBROT_FRACTAL_DRAWER_CLEANUP_H
