#pragma once
#include <iostream>

#define ASSERT(x) if (!(x)){std::cout << "AssertionError " << \
	#x << " at line: " << __LINE__ << " in file: " << __FILE__ << std::endl;\
	exit(1);}
#define GLCall(x) ClearGLErrors();\
        x;\
        ASSERT(CheckGLErrors(#x, __FILE__, __LINE__));

void ClearGLErrors();
unsigned int CheckGLErrors(const char* function,const char* file, int line);
