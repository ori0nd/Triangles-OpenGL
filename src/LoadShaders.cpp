//
//  LoadShaders.cpp
//  Triangles
//
//  Created by Kirill Kravinsky on 2015-10-02.
//  Copyright © 2015 Kirill Kravinsky. All rights reserved.
//

#include "LoadShaders.h"

#include <fstream>
#include <vector>

GLuint LoadShaders(ShaderInfo* shaders)
{
    return create_program(shaders[0].filename, shaders[1].filename);
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
void read_shader_src(const char *fname, std::vector<char> &buffer) {
	std::ifstream in;
	in.open(fname, std::ios::binary);

	if(in.is_open()) {
		// Get the number of bytes stored in this file
		in.seekg(0, std::ios::end);
		size_t length = (size_t)in.tellg();

		// Go to start of the file
		in.seekg(0, std::ios::beg);

		// Read the content of the file in a buffer
		buffer.resize(length + 1);
		in.read(&buffer[0], length);
		in.close();
		// Add a valid C - string end
		buffer[length] = '\0';
	}
	else {
		std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
		exit(-1);
	}
}

// Compile shader
GLuint load_and_compile_shader(const char* fname, GLenum shaderType) {
    std::vector<char> buffer;
    read_shader_src(fname, buffer);
    const char* src = &buffer[0];

    // Compile shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    // Check the result of compilation
    GLint test;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
    if (!test) {
        std::cerr << "Shader compilation failed: " << std::endl;
        std::vector<char> compilation_log(512);
        glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
        std::cerr << &(compilation_log[0]) << std::endl;
        exit(-1);
    }

    return shader;
}

// Create a program from two shaders
GLuint create_program(const char* path_vert_shader, const char* path_frag_shader) {
    GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
    GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return shaderProgram;
}
