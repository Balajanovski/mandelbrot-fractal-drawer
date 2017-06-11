#ifndef MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H
#define MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Get_GL.h"
#include "Pixel_Stream_Base.h"
#include "RGB.h"
#include <memory>

class Screen_Stream : public Pixel_Stream_Base {
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
    Screen_Stream(std::shared_ptr<Bounds2D<int>> &, const std::string &, const std::string &);
    virtual ~Screen_Stream() override;

    void make_current() {
        glfwMakeContextCurrent(screen);
    }

    virtual void flush() override;
};


#endif //MANDELBROT_FRACTAL_DRAWER_DRAW_BUFFER_H
