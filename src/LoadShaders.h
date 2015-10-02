//
//  LoadShaders.h
//  Triangles
//
//  Created by Kirill Kravinsky on 2015-10-02.
//  Copyright © 2015 Kirill Kravinsky. All rights reserved.
//

#ifndef LoadShaders_h
#define LoadShaders_h

#include "vgl.h"

#include <vector>

typedef struct {
    GLenum type;
    const char* filename;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo* shaders);

void read_shader_src(const char *fname, std::vector<char> &buffer);
GLuint load_and_compile_shader(const char* fname, GLenum shaderType);
GLuint create_program(const char* path_vert_shader, const char* path_frag_shader);

#endif /* LoadShaders_h */
