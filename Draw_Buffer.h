#ifndef MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H
#define MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H

#define GLEW_STATIC

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Get_GL.h"
#include "Window.h"
#include "Cleanup.h"

struct RGB {
    float r;
    float g;
    float b;
};

class Draw_Buffer {
friend Draw_Buffer &operator<<(Draw_Buffer &db, const RGB &pixel);
    std::unique_ptr<Window<int>> window;

    std::unique_ptr<GLFWwindow, Cleaner> screen;

    // Pixel buffer
    std::vector<RGB> buffer;

    // Tracks position of appending in buffer
    std::vector<RGB>::iterator pos_iter;

    // Texture where pixels are written to
    GLuint mandelbrot_tex;

    // GLSL Shader program
    GLuint shader_prog;

    // VAO
    GLuint vao;

    // Util function to compile shader
    static void compile_shader(GLuint &shader, const std::string &src);
public:
    Draw_Buffer(Window<int> *, const std::string &, const std::string &);
    ~Draw_Buffer();

    void make_current() {
        glfwMakeContextCurrent(screen.get());
    }

    void flush();

    void keep_window_open();
};

Draw_Buffer &operator<<(Draw_Buffer &db, const RGB &pixel);


#endif //MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H
