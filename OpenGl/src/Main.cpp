#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"



struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

ShaderSource ParseShader(const std::string& filename)
{
	std::ifstream stream(filename);

	std::string line;
	std::stringstream ss[2];
	bool vertexflag = false;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				vertexflag = true;
			else if (line.find("fragment") != std::string::npos)
				vertexflag = false;

		}
		else
		{
			if (vertexflag)
				ss[0] << line << "\n";
			else
				ss[1] << line << "\n";

		}
	}

	return { ss[0].str(), ss[1].str() };
}


////////////////////////////////////////////////////////////////////////


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	// Create Vertex-Fragment Shader Object and get reference to id
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	// Attach Vertex-Fragment Shader source to the Vertex Shader Object
	GLCall(glShaderSource(id, 1, &src, NULL));
	//Compile the Vertex-Fragment Shader into machine code
	GLCall(glCompileShader(id));


	//Error Handling
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	//if error
	if (result == GL_FALSE)
	{
		//length message
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		//stack allocating with alloca()
		char* message = (char*)alloca(length * sizeof(char));
		//error message log
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		//print error
		std::cout << "Failed to compile" << std::endl;
		std::cout << message << std::endl;

	}

	//return shader reference
	return id;
}

static unsigned int CreateShader(const std::string& vertexsource, const std::string& fragmentsource)
{
	unsigned int vertexshader = CompileShader(GL_VERTEX_SHADER, vertexsource);
	unsigned int fragmentshader = CompileShader(GL_FRAGMENT_SHADER, fragmentsource);

	// Create Shader Program Object and get its reference
	GLCall(unsigned int shaderprogram = glCreateProgram());
	// Attach the Vertex and Fragment Shaders to the Shader Program
	GLCall(glAttachShader(shaderprogram, vertexshader));
	GLCall(glAttachShader(shaderprogram, fragmentshader));
	// Wrap-up/Link all the shaders together into the Shader Program
	GLCall(glLinkProgram(shaderprogram));

	//Delete now the useless Vertex and Fragment Objects
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//return shader reference
	return shaderprogram;
}


/////////////////////////////////////////////////



int main()
{
	if (!glfwInit())
		return -1;

	//Tell GLFW which OpenGL verion we're using
	//In out case we're using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Tell GLFW we'ere using Core profile
	//So that mean we only have modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	
		// Vertices coordinates
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, // Lower left corner 0
			0.5f, -0.5f, 0.0f, // lower right corner 1
			-0.5f, 0.5f, 0.0f, // upper corner 2

			0.5f, 0.5f, 0.0f, // 3

		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 1
		};




		//Create a GLFW window object 1920x1080 pixels named "OpenGLapp"
		GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL", NULL, NULL);

		//Check if window failed to be created
		if (!window)
		{
			std::cout << "Failed to create GLFW window!" << std::endl;
			glfwTerminate();
			return -1;
		}

		//Introcude window to current context
		glfwMakeContextCurrent(window);

		//Vsync 
		glfwSwapInterval(1);

		//Load GLAD so it configures OpenGL
		gladLoadGL();

		//Specify the viewport of OpenGL in the window
		//In this case the viewport goes form x = 0, y = 0, to x = 1920, to y = 1080
		GLCall(glViewport(0, 0, 1920, 1080));

		//Pint OpenGL version
		std::cout << glGetString(GL_VERSION) << std::endl;




	{
		//Shaders
		//Parse shaders from file
		ShaderSource shadersource = ParseShader("res/shaders/Basic.shader");
		//Create Shader Program Object
		unsigned int shaderProgram = CreateShader(shadersource.VertexSource, shadersource.FragmentSource);
		// Tell OpenGL which Shader Program we want to use
		GLCall(glUseProgram(shaderProgram));



		// Create reference containers for the Vertex Array Object
		// and the Vertex Buffer Object and Indices Buffer Object (order is important!)
		unsigned int VAO;

		//Generate the VAO , VBO and IBO with only 1 Object each
		GLCall(glGenVertexArrays(1, &VAO));
		//Bind the VAO 
		GLCall(glBindVertexArray(VAO));

		// Create Vertex Buffer Object
		VertexBuffer VBO(vertices, sizeof(vertices));
		//Configure the Vertex Attribute so that OpenGL knows how to read the VBO
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
		//Enable the Vertex Attributes so that OpenGL knows to use it
		GLCall(glEnableVertexAttribArray(0));


		//Create Index Buffer Object
		IndexBuffer IBO(indices, 6);







		//Bind both the VBO and VAO to 0 so that we don't accidentaly modify the VAO and VBO
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		//glBindVertexArray(0);

		// Bind the VAO so OpenGL knows how to use it
		//GLCall(glBindVertexArray(VAO));


		// Uniform Shaders
		// Retrieve location shader of "u_Color"
		GLCall(int location = glGetUniformLocation(shaderProgram, "u_Color"));
		ASSERT(location != -1);
		// Set values color shaders
		GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

		float r = 0.0f;
		float r_increment = 0.005f;

		//Main loop
		while (!glfwWindowShouldClose(window))
		{
			//Specify color of backround
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			//Clean back buffer and assign new color to it
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			// Set values uniform color shaders(Per Draw)
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

			// Draw the triangle/square using the GL_TRIANGLES
			//GLCall detects errors
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

			//Red colors logic
			if (r > 1.0f)
				r_increment = -0.005f;
			else if (r < 0.0f)
				r_increment = 0.005f;
			//Change red every frame
			r += r_increment;

			//Swap the back buffer with the front buffer
			glfwSwapBuffers(window);
			//Handle all GLFW events
			glfwPollEvents();
		}


		//Delete all the Object we've created
		glDeleteVertexArrays(1, &VAO);

		//Delete ShaderProgram
		glDeleteProgram(shaderProgram);

	}//added scope so VBOs IBOs delete before OpenGL termination

	//Delete window before ending program
	glfwDestroyWindow(window);
	
	//End GLFW
	glfwTerminate();




	return 0;
}