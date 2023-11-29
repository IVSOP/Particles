#include "GLErrors.h"

#include <signal.h>
#include <iostream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLDBLACK   "\033[1m\033[30m"
#define BOLDRED     "\033[1m\033[31m" 
#define BOLDGREEN   "\033[1m\033[32m" 
#define BOLDYELLOW  "\033[1m\033[33m" 
#define BOLDBLUE    "\033[1m\033[34m" 
#define BOLDMAGENTA "\033[1m\033[35m" 
#define BOLDCYAN    "\033[1m\033[36m" 
#define BOLDWHITE   "\033[1m\033[37m" 

void GLClearError() {
	while (glGetError());
}

bool GLLogCall(const char *function, const char *file, int line) {
	GLenum error;
	while ((error = glGetError())) {
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

// void MessageCallback( GLenum source,
//                  GLenum type,
//                  GLuint id,
//                  GLenum severity,
//                  GLsizei length,
//                  const GLchar* message,
//                  const void* userParam )
// {
//   fprintf( stderr, "GL CALLBACK: %s\ntype = 0x%x\nseverity = 0x%x\nmessage = %s\n",
//            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
//             type, severity, message );
// }

void checkErrorInShader(GLuint shader) {
	int res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (!res) {
		int len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		//dynamic allocation on the stack
		//ele nao pôs o +1
		char *message = (char *)alloca((len + 1) * sizeof(char));
		glGetShaderInfoLog(shader, len, &len, message);
		// std::cout << "Shader compilation failed for " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		print_error("Shader compilation failed"); // can I use %s here???????
		std::cout << message << std::endl;
		glDeleteShader(shader);
		raise(SIGINT);
	}
}

void APIENTRY openglCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cout << "---------------------opengl-callback-start------------" << std::endl;
    std::cout << "message: "<< message << std::endl;
    std::cout << "type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "OTHER";
        break;
    }
    std::cout << std::endl;

    std::cout << "id: " << id << std::endl;
    std::cout << "severity: ";
    switch (severity){
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << YELLOW << "LOW" << RESET;
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << YELLOW << "MEDIUM" << RESET;
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << RED << "HIGH" << RESET;
        break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << GREEN << "NOTIFICATION" << RESET;
		break;
    }
    std::cout << std::endl;
    std::cout << "---------------------opengl-callback-end--------------" << std::endl;
}

// thse two are basically the same, will remake this
void validateProgram(const GLuint program) {
	GLCall(glValidateProgram(program));
	GLint params = 0;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &params);
	if (params == GL_FALSE) {
		print_error("Program not valid");
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		// GLchar infoLog[maxLength];
		GLchar *infoLog = static_cast<GLchar *>(alloca(maxLength * sizeof(GLchar)));
		infoLog[0] = '\0';
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// The program is useless now. So delete it.
		glDeleteProgram(program);

		// got lazy
		print_error(infoLog);

		raise(SIGINT);
	}
}

void checkProgramLinking(const GLuint program) {
	GLCall(glValidateProgram(program));
	GLint params = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (params == GL_FALSE) {
		print_error("Program linking failed");
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		// GLchar infoLog[maxLength];
		GLchar *infoLog = static_cast<GLchar *>(alloca(maxLength * sizeof(GLchar)));
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// The program is useless now. So delete it.
		glDeleteProgram(program);

		// got lazy
		print_error(infoLog);

		raise(SIGINT);
	}
}
