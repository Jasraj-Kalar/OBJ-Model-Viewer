#include <stdio.h>

#include <glad/glad.h>

#include "getGLErrors.h"


void glGetErrors(int line, const char * file)
{
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
        printf("Error: %u, On Line: %i, In File: %s\n", err, line, file);
}