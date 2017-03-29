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
    GLbyte r;
    GLbyte g;
    GLbyte b;
};

class Draw_Buffer {
friend Draw_Buffer &operator<<(Draw_Buffer &db, const RGB &pixel);
    // Pointer to glfw screen
    GLFWwindow *screen;

    // Represents OpenGL context dimensions
    std::unique_ptr<Window<int>> window;

    // Pixel buffer
    std::vector<RGB> buffer;

    // Tracks position of appending in buffer
    std::vector<RGB>::iterator pos_iter;

    // Texture where pixels are written to
    GLuint mandelbrot_tex;

    // GLSL Shader program
    GLuint shader_prog;

    // Vertex shader
    GLuint vertex_shader;

    // Fragment shader
    GLuint frag_shader;

    // VAO
    GLuint vao;

    // Element buffer object
    GLuint ebo;

    // Vertex buffer object
    GLuint vbo;

    // Util function to compile shader
    static void compile_shader(GLuint &shader, const std::string &src);
public:
    Draw_Buffer(Window<int> *, const std::string &, const std::string &);
    ~Draw_Buffer();

    void make_current() {
        glfwMakeContextCurrent(screen);
    }

    void flush();

    void keep_window_open();
};

Draw_Buffer &operator<<(Draw_Buffer &db, const RGB &pixel);


#endif //MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H
