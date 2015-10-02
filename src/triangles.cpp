//
//  Main File
//  main.cpp
//
//  Author: Kirill "ori0n" Kravinsky 2015
//

#include "vgl.h"
#include "LoadShaders.h"

#include <vector>
#include <iostream>

#include <FreeImage.h>

using namespace std;

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

// Main window handle
GLFWwindow* wnd;

void load_image(const char*);

void display()
{
    // Clear buffer
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);

    // Swap buffers (using double buffering)
    glfwSwapBuffers(wnd);
}

void wnd_resized_callback(GLFWwindow* window, int width, int height)
{
    glClearColor(0, 0, 0, 1);
    
    glViewport(0, 0, width, height);
    
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(wnd);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void initialize() {
    // Use a vertex array object
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);

    // 4 triangles to be rendered
    GLfloat vertices[NumVertices][2] = {
        { -0.90, -0.90 },  // Triangle 1
        {  0.85, -0.90 },
        { -0.90,  0.85 },
        {  0.90, -0.85 },  // Triangle 2
        {  0.90,  0.90 },
        { -0.85,  0.90 }
    };
    
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER, "resources/triangles.vert" },
        { GL_FRAGMENT_SHADER, "resources/triangles.frag" },
        { GL_NONE, NULL }
    };
    
    GLuint program = LoadShaders(shaders);
    glUseProgram(program);
    
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}

void load_image(const char* fname)
{
    // For static linking only
#ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
#endif
    
    FIBITMAP* bitmap;
    
    // Get format of img file
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fname);
    
    // If format cannot be determined, try to guess from filename
    if (fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(fname);
    
    // Load data to bitmap
    if (fif != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fif))
        bitmap = FreeImage_Load(fif, fname);
    else
        bitmap = NULL;
    
    // Process image
    if (bitmap) {
        unsigned int w = FreeImage_GetWidth(bitmap);
        unsigned int h = FreeImage_GetHeight(bitmap);
        unsigned pixel_size = FreeImage_GetBPP(bitmap);
        
        // Get pointer to pixel data
        BYTE* data = (BYTE*)FreeImage_GetBits(bitmap);
        
        // Process only RGB & RGBA images
        if (pixel_size == 24)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
        else if (pixel_size == 32)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
        else {
            std::cerr << "Pixel size unsupported, only RGB/RGBA\n";
            exit(EXIT_FAILURE);
        }
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cerr << "unable to load texture file \"" << fname << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    FreeImage_Unload(bitmap);
    
#ifdef FREEIMAGE_LIB
    FreeImage_DeInitialise();
#endif
}

int main(int argc, char** argv)
{
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwSetErrorCallback(error_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Main window handle
    wnd = glfwCreateWindow(1920, 1200, "GLFW Bootstrap", glfwGetPrimaryMonitor(), NULL);
    
    if (!wnd)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(wnd);
    glfwSwapInterval(1);
    
    glfwSetWindowSizeCallback(wnd, wnd_resized_callback);
    glfwSetKeyCallback(wnd, key_callback);
    
#ifndef __APPLE__
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (!glewInit()) {
        cerr << "unable to load glew library, fatal" << endl;
        exit(EXIT_FAILURE);
    }
#endif
    
    // Print OpenGL version
    int major = glfwGetWindowAttrib(wnd, GLFW_CONTEXT_VERSION_MAJOR);
    int minor = glfwGetWindowAttrib(wnd, GLFW_CONTEXT_VERSION_MINOR);
    int rev =   glfwGetWindowAttrib(wnd, GLFW_CONTEXT_REVISION);
    std::cout << "OpenGL - " << major << "." << minor << "." << rev << std::endl;
    
    
    // Initialize a vertex array orbject
    initialize();
    
    while (!glfwWindowShouldClose(wnd))
    {
        display();
        glfwPollEvents();
    }
    
    glfwDestroyWindow(wnd);
    glfwTerminate();
    
    exit(EXIT_SUCCESS);
}
