#include <stdio.h>
#include "stdlib.h"
#include "Shader.h"
#include <string.h>

/**
 * Shader constructor
 */
Shader::Shader(void) {
	shaderProgramid = 0;
	vertShaderid = 0;
	fragShaderid = 0;
}

/**
 * Shader destructor
 */
Shader::~Shader(void) {
	// destruct
}

/**
 * Read the shader code from a file.
 *
 * @param  fileName Filename to be read
 * @return          Shader code as a char *
 */
char * Shader::readCode(char * fileName) {
	char * shaderCode = NULL;
	int codeLength = 0;
	FILE *fp = NULL;

    // check for error in file name

    fp = fopen(fileName, "r");	// open file and check for errors
    if ( fp == NULL ) { return NULL; }

     // fine the length of code
    fseek(fp, 0L, SEEK_END);
    codeLength = ftell(fp);
    rewind(fp);		// could use fseek(fp, 0L, SEEK_SET)

	if (codeLength > 0) {
		// allocated space for code and check for errors
		shaderCode = (char *) malloc(codeLength+1);	// add a space for the '\0'
		if (shaderCode == NULL) return(NULL);
		memset((void *) shaderCode, 0, codeLength+1);
		fread((void *) shaderCode, sizeof(char), codeLength, fp);
		shaderCode[codeLength] = 0;
	}

	if (fp != NULL) fclose(fp);

	return shaderCode;
}

/**
 * Create the shaders.
 *
 * @param  fileName   file name of shader. Path is assume to be the project.
 * @param  shaderType Can either be vertex shader or fragment shader.  The type
 *                    must correspond to the OpenGL shader types.
 * @param  shaderid   Shader identifier pointer.
 * @return            0 - if successful, -1 if error.
 */
int Shader::createShaderObj(char* fileName , int shaderType, GLuint *shaderid) {
	char *code = NULL;
	int rc = 0;


	// create a shader handle
	*shaderid = glCreateShader(shaderType);

	// read the code
	code = readCode(fileName);
	if (code == NULL) return(-1);

	// attach the source to the shaders
	glShaderSource(*shaderid, 1, &code, NULL);

	// compile the code
	glCompileShader(*shaderid);;

	// check for errors
	glGetShaderiv(*shaderid, GL_COMPILE_STATUS, &rc);
	if (rc != GL_TRUE) {
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		fprintf(stderr,"Error when creating a shader \n");
		//fputs("Error when creating a shader \n",stderr);
		glGetShaderiv(*shaderid, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *) malloc(bufSize*sizeof(char)+1);
		if (error != NULL) {
			glGetShaderInfoLog(*shaderid, bufSize, &length, error);
			fprintf(stderr,"%s \n", error);
		}
		if (error != NULL) free(error);
		rc = -1;
	} else rc = 0;

	if (code != NULL) free(code);	// free the source code of the shader

	return(rc);

}

/**
 * Creates a shader program. The two shader programs (vertex and
 * fragment) were already compiled.
 *
 * @param  vertShaderid A handle to the vertex shader
 * @param  fragShaderid A handle to the fragment shader
 * @param  shaderProgId A handle to the shader program.
 * @return              0 if successful, -1 if error
 */
GLint Shader::createShaderProgram(GLint vertShaderid, GLint fragShaderid, GLuint *shaderProgId) {

	int rc = 0;
	// get a handle to the shader program
	shaderProgramid = glCreateProgram();
	// connect the shaders subprogram to the "main" program
	glAttachShader(shaderProgramid, vertShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr,"error in attach shaders \n");
		rc = -1;
		goto err;
	}

	glAttachShader(shaderProgramid, fragShaderid);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr,"error in attach shaders \n");
		rc = -1;
		goto err;
	}

	// bind a varialbe before linking
	//glBindAttribLocation(shaderProgramid, 7, "testNormal");
	glLinkProgram(shaderProgramid);

	// check for errors

	glGetProgramiv(shaderProgramid, GL_LINK_STATUS, &rc);

	if (rc != GL_TRUE) {
		rc = -1;
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		fprintf(stderr,"Error when creating a shader program \n");
		//fputs("Error when creating a shader \n",stderr);
		glGetProgramiv(shaderProgramid, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *) malloc(bufSize*sizeof(char)+1);
		if (error != NULL) {
			glGetProgramInfoLog(shaderProgramid, bufSize, &length, error);
			fprintf(stderr,"%s \n", error);
		}
		if (error != NULL) free(error);
	} else rc = 0;

	shaderStatus();  // may want to remove it.

	if (shaderProgId != NULL) *shaderProgId = shaderProgramid;
err:
	return (rc);
}

/**
 * Creates a shader program from files vsFileName and fsFileName.
 *
 * @param  vsFileName      Filename of vertex shader.
 * @param  fsFileName      Filename of fragment shader.
 * @param  shaderProgramid A handle to the shader program.
 * @return                 0 if successful !=0 if error.
 */
int Shader::createShaderProgram(char * vsFileName, char * fsFileName, unsigned int *shaderProgramid) {

	int rc = 0;

	rc = createShaderObj(vsFileName, GL_VERTEX_SHADER, &vertShaderid);
	if (rc == 0) {
		rc = createShaderObj(fsFileName, GL_FRAGMENT_SHADER, &fragShaderid);
	}
	if (rc == 0) {
		rc = createShaderProgram(vertShaderid, fragShaderid,shaderProgramid);
	}

	return(rc);
}

/**
 * Print the status and some information about the linked shader program
 * @return  0 if success
 */
int Shader::shaderStatus(void) {
	int rc;

	glGetProgramiv(shaderProgramid, GL_VALIDATE_STATUS , &rc);
	if (rc == 0) {
		fprintf(stderr,"shader is valid status = %d \n", rc);
	} else {
		fprintf(stderr,"shader is not valid status = %d \n", rc);
	}

	rc = 0;
	glGetProgramiv(shaderProgramid, GL_INFO_LOG_LENGTH, &rc);
	if (rc == 0) {
		fprintf(stderr,"shader program info log length= %d \n", rc);
	} else {
		char s[1024];
		int length = 0;
		s[0]=0;
		glGetProgramInfoLog(shaderProgramid, 1024, &length, s);
		fprintf(stderr,"shader info log = %s \n", s);
	}

	glGetProgramiv(shaderProgramid, GL_ATTACHED_SHADERS, &rc);
	fprintf(stderr,"shader number ofattached shaders= %d \n", rc);
	glGetProgramiv(shaderProgramid, GL_ACTIVE_ATTRIBUTES, &rc);
	fprintf(stderr,"shader number of active attributes= %d \n", rc);
	glGetProgramiv(shaderProgramid, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH , &rc);
	fprintf(stderr,"shader maximum name length of an active attriute= %d \n", rc);
	glGetProgramiv(shaderProgramid, GL_ACTIVE_UNIFORMS, &rc);
	fprintf(stderr,"shader number of active uniforms= %d \n", rc);
	glGetProgramiv(shaderProgramid, GL_ACTIVE_UNIFORM_MAX_LENGTH, &rc);
	fprintf(stderr,"shader maximum name length of an active uniform= %d \n", rc);

	return 0;
}
