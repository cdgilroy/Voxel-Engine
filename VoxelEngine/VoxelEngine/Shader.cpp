#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.h"

// Basic file reading algorithm
// Reads file & returns content as string
std::string readFile(const std::string& filename)
{
	std::ifstream file(filename);		// Create input file stream
	std::stringstream buffer;			// Create buffer	
	buffer << file.rdbuf();				// Read file into buffer
	std::string content = buffer.str();	// Convert buffer to string
	
	return content;
}

// Reads in shader file and returns compiled shader
GLuint loadShader(const std::string& filename, GLenum type)
{
	std::string fileContent = readFile(filename);	// Read shader file
	GLuint shader = glCreateShader(type);			// Create shader
	const char* source = fileContent.c_str();		// Create source from file
	glShaderSource(shader, 1, &source, 0);			// Set shader source
	glCompileShader(shader);						// Compile shader

	// IF shader hasn't compiled, print error data
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei length;
		char log[1024];
		glGetShaderInfoLog(shader, 1024, &length, log);
		std::cout << "Could not compile shader: " << shader << ": " << filename << std::endl;
		std::cout << log << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	
	return shader;
}

// Create shader program from shaders
GLuint createProgram(GLuint* shaders, int count)
{
	GLuint program = glCreateProgram();			// Create program
	for (int i = 0; i < count; ++i)				// Attach shaders to program
		glAttachShader(program, shaders[i]);
	glLinkProgram(program);						// Link the program

	// If shader not linked, print error data
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei length;
		char log[1024];
		glGetProgramInfoLog(program, 1024, &length, log);
		std::cout << "Error linking program." << std::endl;
		std::cout << log << std::endl;
		for (int i = 0; i < count; ++i)
			glDetachShader(program, shaders[i]);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}