#ifndef MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H
#define MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H

#define GLEW_STATIC

#include <memory>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Get_GL.h"
#include "Buffer_Base.h"
#include "RGB.h"

class Draw_Buffer : public Buffer_Base<RGB> {
    // Pointer to glfw screen
    GLFWwindow *screen;

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
    virtual ~Draw_Buffer() override;

    void make_current() {
        glfwMakeContextCurrent(screen);
    }

    virtual void flush() override;
};


#endif //MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H
