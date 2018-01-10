#include "Screen_Stream.h"
#include "Pixel_Stream_Base.h"
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

#define NDEBUG
#include <cassert>

// Util function to compile a shader from source
void Screen_Stream::compile_shader(GLuint &shader, const std::string &src) {
    std::ifstream is(src);
    std::string code;

    std::string temp_str;
    while (std::getline(is, temp_str)) {
        code += temp_str + '\n';
    }

    const char *c_code = code.c_str();
    glShaderSource(shader, 1, &c_code, NULL);
    glCompileShader(shader);
}




void Screen_Stream::generate_shaders(const std::string &vertex_shader_src, const std::string &frag_shader_src) {
    // Generate shaders
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    assert(glGetError() == GL_NO_ERROR);

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    assert(glGetError() == GL_NO_ERROR);

    GLint compile_status;
    compile_shader(vertex_shader, vertex_shader_src);
    assert(glGetError() == GL_NO_ERROR);

#ifndef NDEBUG
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, buffer);
        throw std::runtime_error(buffer);
    }
#endif

    compile_shader(frag_shader, frag_shader_src);
    assert(glGetError() == GL_NO_ERROR);

#ifndef NDEBUG
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(frag_shader, 512, NULL, buffer);
        throw std::runtime_error(buffer);
    }
#endif

// Put shaders into shader program
    shader_prog = glCreateProgram();
    assert(glGetError() == GL_NO_ERROR);

    glAttachShader(shader_prog, vertex_shader);
    assert(glGetError() == GL_NO_ERROR);

    glAttachShader(shader_prog, frag_shader);
    assert(glGetError() == GL_NO_ERROR);

    glBindFragDataLocation(shader_prog, 0, "outColor");
    assert(glGetError() == GL_NO_ERROR);

    glLinkProgram(shader_prog);
    assert(glGetError() == GL_NO_ERROR);

    glUseProgram(shader_prog);
    assert(glGetError() == GL_NO_ERROR);

#ifndef NDEBUG
    GLint success;
    GLchar info_log[512];

    glGetProgramiv(shader_prog, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_prog, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
#endif
}



Screen_Stream::Screen_Stream(std::shared_ptr<Bounds2D<int>> &bnds, const std::string &vertex_shader_src, const std::string &frag_shader_src) :
                        Pixel_Stream_Base(bnds) {
// Initialise GLFW
    if (!glfwInit()) {
        throw std::runtime_error("error: GLFW unable to initialise");
    }

// Set up the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    screen = (glfwCreateWindow(bounds->width(), bounds->height(), "Mandelbrot Fractal", nullptr, nullptr));

    make_current();

// Initialise glew
    glewExperimental = GL_TRUE;
    GLenum glewinit = glewInit();

#ifndef NDEBUG
    if (glewinit != GLEW_OK) {
        std::ostringstream ss;
        ss << "error: Glew unable to initialise" << glewinit;
        throw std::runtime_error(ss.str());
    }
#endif
    assert(glGetError() == GL_NO_ERROR);

// Clear buffer
    glClearColor(0, 0, 0, 0);
    assert(glGetError() == GL_NO_ERROR);

    glClear(GL_COLOR_BUFFER_BIT);
    assert(glGetError() == GL_NO_ERROR);

    generate_shaders(vertex_shader_src, frag_shader_src);

// Create VAO
    glGenVertexArrays(1, &vao);
    assert(glGetError() == GL_NO_ERROR);

    glBindVertexArray(vao);
    assert(glGetError() == GL_NO_ERROR);

// Create vertex and element buffers
    const static GLfloat vertices[] = {
            // Position   Tex-coords
            -1.0f,  1.0f, 0.0f, 0.0f, // Top-left
             1.0f,  1.0f, 1.0f, 0.0f, // Top-right
             1.0f, -1.0f, 1.0f, 1.0f, // Bottom-right
            -1.0f, -1.0f, 0.0f, 1.0f  // Bottom-left
    };

    glGenBuffers(1, &vbo);
    assert(glGetError() == GL_NO_ERROR);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    assert(glGetError() == GL_NO_ERROR);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);

    const static GLuint elements[] = {
            0, 1, 2,
            2, 3, 0
    };

    glGenBuffers(1, &ebo);
    assert(glGetError() == GL_NO_ERROR);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    assert(glGetError() == GL_NO_ERROR);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    assert(glGetError() == GL_NO_ERROR);

// Set shader attributes
    GLint pos_attrib = glGetAttribLocation(shader_prog, "position");
    assert(glGetError() == GL_NO_ERROR);

    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    assert(glGetError() == GL_NO_ERROR);

    glEnableVertexAttribArray(pos_attrib);
    assert(glGetError() == GL_NO_ERROR);

    GLint tex_coord_attrib = glGetAttribLocation(shader_prog, "tex_coord");
    assert(glGetError() == GL_NO_ERROR);

    glEnableVertexAttribArray(tex_coord_attrib);
    assert(glGetError() == GL_NO_ERROR);

    glVertexAttribPointer(tex_coord_attrib, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    assert(glGetError() == GL_NO_ERROR);

// Generate texture
    glGenTextures(1, &mandelbrot_tex);
    assert(glGetError() == GL_NO_ERROR);

// Bind the texture information

    glActiveTexture(GL_TEXTURE0);
    assert(glGetError() == GL_NO_ERROR);

    glBindTexture(GL_TEXTURE_2D, mandelbrot_tex);
    assert(glGetError() == GL_NO_ERROR);

    glUniform1i(glGetUniformLocation(shader_prog, "tex"), 0);
    assert(glGetError() == GL_NO_ERROR);
}




Screen_Stream::~Screen_Stream() {

// Unbind buffer
    glBindVertexArray(0);
    assert(glGetError() == GL_NO_ERROR);

// Delete shaders
    glDeleteProgram(shader_prog);
    assert(glGetError() == GL_NO_ERROR);

    glDeleteShader(vertex_shader);
    assert(glGetError() == GL_NO_ERROR);

    glDeleteShader(frag_shader);
    assert(glGetError() == GL_NO_ERROR);

// Delete buffers
    glDeleteBuffers(1, &vbo);
    assert(glGetError() == GL_NO_ERROR);

    glDeleteBuffers(1, &ebo);
    assert(glGetError() == GL_NO_ERROR);

    glDeleteVertexArrays(1, &vao);
    assert(glGetError() == GL_NO_ERROR);

// Terminate GLFW
    glfwDestroyWindow(screen);
    glfwTerminate();
}




void Screen_Stream::flush() {
    glClear(GL_COLOR_BUFFER_BIT);
    assert(glGetError() == GL_NO_ERROR);

    // Reset texture
    glBindTexture(GL_TEXTURE_2D, mandelbrot_tex);
    assert(glGetError() == GL_NO_ERROR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bounds->width(), bounds->height(), 0, GL_RGB, GL_BYTE, &get_buffer()[0]);
    assert(glGetError() == GL_NO_ERROR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    assert(glGetError() == GL_NO_ERROR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    assert(glGetError() == GL_NO_ERROR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    assert(glGetError() == GL_NO_ERROR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    assert(glGetError() == GL_NO_ERROR);

    // Draw rectangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    assert(glGetError() == GL_NO_ERROR);

    // Swap buffers
    glfwSwapBuffers(screen);

    // Reset iterator
    pos_iter = get_buffer().begin();

    while(!glfwWindowShouldClose(screen)) {
        if ((glfwGetKey(screen, GLFW_KEY_ESCAPE) == GLFW_PRESS) || (glfwGetKey(screen, GLFW_KEY_ESCAPE) == GLFW_REPEAT)) {
            glfwSetWindowShouldClose(screen, true);
        }
        glfwPollEvents();
    }
}