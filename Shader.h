#ifndef RYAN_SHADER
#define RYAN_SHADER


#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include "glew.h"
#include <GL/glut.h>
#include <windows.h>
#endif

class Shader {
public:
	Shader(void);
	~Shader(void);

	char * readCode(char * fileName);
	int createShaderObj(char* fileName , int shaderType, GLuint *shaderid);

	// functions creates a shader program.  The two shader programs (vertex and fragment) were already compiled.
	GLint createShaderProgram(GLint vertShaderid, GLint fragShaderid, GLuint *shaderProgId);
	GLuint shaderProgramid;
	// creates a shader program from files vsFileName and fsFileName
	int createShaderProgram(char * vsFileName, char * fsFileName, unsigned int *shaderProgramid);
	int shaderStatus(void);
private:
	GLuint vertShaderid;
	GLuint fragShaderid;
};

#endif
