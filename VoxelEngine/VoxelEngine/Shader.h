#pragma once

#include <GL\glew.h>
#include <string>

// Utility code for working with shaders
// Delcare loadShader & createProgram methods
GLuint loadShader(const std::string& filename, GLenum type);
GLuint createProgram(GLuint* shaders, int count);