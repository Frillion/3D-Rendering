#pragma once
#include <iostream>
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) exit(1)
#define GLCall(x) ClearGLErrors();\
        x;\
        ASSERT(CheckGLErrors(#x, __FILE__, __LINE__));


void ClearGLErrors();
unsigned int CheckGLErrors(const char* function,const char* file, int line);
