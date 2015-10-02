//
//  vgl.h
//  Triangles
//
//  Created by Kirill Kravinsky on 2015-10-02.
//  Copyright © 2015 Kirill Kravinsky. All rights reserved.
//

#ifndef vgl_h
#define vgl_h

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#else
#include <GL/glew.h>
#endif
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define BUFFER_OFFSET(offset) ((void *) (offset))

#endif /* vgl_h */
