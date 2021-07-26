#pragma once

#include <glad/glad.h>

//OpenGL Error Checking
// GLCall(x) 
#ifdef _DEBUG
#define ASSERT(x) if (!(x)) __debugbreak();// if x must be into ()
#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))// (#) converts x to string , file path and code line error
#else
#define GLCall(x) x
#endif // DEBUG


//Clear errors
void GLClearError();
//Check error and return false if found 
bool GLLogCall(const char* function, const char* file, int line);
