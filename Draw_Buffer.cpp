#include "Draw_Buffer.h"
#include "Buffer_Base.h"
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

// Util function to compile a shader from source
static void Draw_Buffer::compile_shader(GLuint &shader, const std::string &src) {
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

Draw_Buffer::Draw_Buffer(Window<int> *win, const std::string &vertex_shader_src, const std::string &frag_shader_src) :
        Buffer_Base(win) {
// Initialise GLFW
    if (!glfwInit()) {
        throw std::runtime_error("error: GLFW unable to initialise");
    }

// Set up the window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    screen = (glfwCreateWindow(win->width(), win->height(), "Mandelbrot Fractal", nullptr, nullptr));

    make_current();

// Initialise glew
    glewExperimental = GL_TRUE;
    GLenum glewinit = glewInit();

    if (glewinit != GLEW_OK) {
        std::ostringstream ss;
        ss << "error: Glew unable to initialise" << glewinit;
        throw std::runtime_error(ss.str());
    }

// Clear
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

// Generate shaders
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    GLint compile_status;
    compile_shader(vertex_shader, vertex_shader_src);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vertex_shader, 512, NULL, buffer);
        throw std::runtime_error(buffer);
    }

    compile_shader(frag_shader, frag_shader_src);
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(frag_shader, 512, NULL, buffer);
        throw std::runtime_error(buffer);
    }

// Put shaders into shader program
    shader_prog = glCreateProgram();
    glAttachShader(shader_prog, vertex_shader);
    glAttachShader(shader_prog, frag_shader);
    glBindFragDataLocation(shader_prog, 0, "outColor");
    glLinkProgram(shader_prog);
    glUseProgram(shader_prog);

// Create VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

// Create vertex and element buffers
    const static GLfloat vertices[] = {
            // Position   Tex-coords
            -1.0f,  1.0f, 0.0f, 0.0f, // Top-left
             1.0f,  1.0f, 1.0f, 0.0f, // Top-right
             1.0f, -1.0f, 1.0f, 1.0f, // Bottom-right
            -1.0f, -1.0f, 0.0f, 1.0f  // Bottom-left
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    const static GLuint elements[] = {
            0, 1, 2,
            2, 3, 0
    };

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

// Set shader attributes
    GLint pos_attrib = glGetAttribLocation(shader_prog, "position");
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(pos_attrib);

    GLint tex_coord_attrib = glGetAttribLocation(shader_prog, "tex_coord");
    glEnableVertexAttribArray(tex_coord_attrib);
    glVertexAttribPointer(tex_coord_attrib, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

// Generate texture
    glGenTextures(1, &mandelbrot_tex);

// Bind the texture information

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mandelbrot_tex);
    glUniform1i(glGetUniformLocation(shader_prog, "tex"), 0);
}

Draw_Buffer::~Draw_Buffer() {

// Unbind buffer
    glBindVertexArray(NULL);

// Delete shaders
    glDeleteProgram(shader_prog);
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

// Delete buffers
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);

// Terminate GLFW
    glfwDestroyWindow(screen);
    glfwTerminate();
}

void Draw_Buffer::flush() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Reset texture
    glBindTexture(GL_TEXTURE_2D, mandelbrot_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window->width(), window->height(), 0, GL_RGB, GL_BYTE, &buffer[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Draw rectangle
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Sends message if there is an OpenGL bug
    GLenum err = glGetError();
    if (err) {
        std::stringstream ss;
        ss << "GL Error: " << err;
        throw std::runtime_error(ss.str());
    }

    // Swap buffers
    glfwSwapBuffers(screen);

    // Reset iterator
    pos_iter = buffer.begin();

    while(!glfwWindowShouldClose(screen)) {
        glfwPollEvents();
    }
}