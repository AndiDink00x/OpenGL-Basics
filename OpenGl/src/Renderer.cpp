#include "Renderer.h"
#include <iostream>



void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);

}

bool GLLogCall(const char* function, const char* file, int line)
{
	bool ErrorFlag = false;
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << " : " << line << std::endl;
		ErrorFlag = true;

	}
	if (ErrorFlag)
		return false;
	return true;
}