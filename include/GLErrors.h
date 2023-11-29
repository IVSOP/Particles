#ifndef GLERRORS_H
#define GLERRORS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <signal.h>

#define print_error(msg) printf("%s, %s, line %d:\n", __FILE__, __func__, __LINE__); perror(msg);

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(f) GLClearError();\
	f;\
	ASSERT(GLLogCall(#f, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char *function, const char *file, int line);

void checkErrorInShader(GLuint shader);

void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

void validateProgram(const GLuint program);
void checkProgramLinking(const GLuint program);

#endif
