//=============================================================================
// skybox.cpp
//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose:
//--------------
// a. demonstarte the usage of a skybox
//
//
// Description:
//--------------
// A simple application that demonstrates how to crete and use a skybox
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

#include "SkyBox.h"

#include "stdio.h"
#include "stdlib.h"
#include "SOIL.h"
#include "ryan_matrix.h"

// helper function - move to a new file gl_util

static int checkError()
{
  int rc = glGetError();
  switch (rc) {
  case GL_NO_ERROR:
    rc = 0;
    break;
  case GL_INVALID_ENUM:
    printf("gl error - not a legal enum \n");
    rc = 1;
    break;
  case GL_INVALID_VALUE:
    printf("gl error - not a legal value \n");
    rc = 2;
    break;

  case GL_INVALID_OPERATION:
    printf("gl error - not a legal operation \n");
    rc = 3;
    break;


  case GL_INVALID_FRAMEBUFFER_OPERATION:
    printf("gl error - stack underflow \n");
    rc = 5;
    break;

  case GL_OUT_OF_MEMORY:
    printf("gl error - out of memory \n");
    rc = 6;
    break;


  default:
    printf("gl error - default \n");
    rc = 9;
    break;
  }
  return(rc);
}

SkyBox::SkyBox(void) : texHandle(0),s() {
}


SkyBox::~SkyBox(void) {
}

static int loadSkyboxImage(char *texFileName, GLuint target) {
  int rc = 0;
  int width=1, height=1;
  unsigned char* image;
  int channels;

  image = SOIL_load_image(texFileName, &width, &height, &channels, SOIL_LOAD_RGB);
  glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  rc = checkError();
  return(rc);
}

int SkyBox::loadSkybox(char ** texFileName) {
  int rc;
  int i;
  int width=1, height=1;

  if (texFileName == NULL) return(-1);
  else for (i = 0; i < 6; i++) {
    if (texFileName[i] == NULL) return(-1);
  }

  //glEnable(GL_TEXTURE_CUBE_MAP);
  glActiveTexture(GL_TEXTURE3);

  // Create texture object
  glGenTextures(1, &texHandle);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texHandle);
  checkError();

  for (i = 0; i < 6; i++) {
    rc = loadSkyboxImage(texFileName[i],GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);   // the targets defines are consecutives
    if (rc != 0) {
      printf("error loading skybox textures \n");
      return(-1);
    }
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);
  checkError();

  return (0);
}

void SkyBox::displaySkybox(Camera cam) {

  int t = 1;  // remove
  glDisable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects

  Matrix4f viewMat, projMat, modelMat, m;

  // set local camera for the skybox
  // Camera c1 = cam;
  //c1.changeAbsPoition(0,0,0);
  //c1.changeLookAtVector(1, 1, 1);

  //glFrontFace(GL_CW);
  //glCullFace(FRONT);

  // set up the mode to wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //set up the mode to fill
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Vector3f camPos = cam.getPosition();

  // get the transformation matrix
  modelMat = Matrix4f::translation(camPos.x, camPos.y, camPos.z);

  // setting up the viewpoint transformation
  viewMat = cam.getViewMatrix();

  // setting up the projection transformation  make sure that it is the same
  // projection as the display function!!!
  //projMat =   Matrix4f::symmetricPerspectiveProjectionMatrix(30, 1, 0.1, 500);
  projMat = cam.getProjMatrix();

  // frustum matrix
  //projMat = Matrix4f::frustumProjectionMatrix(-1,-1,1,1, 10,100);

  // putting it all together
  m = projMat * viewMat * modelMat;

  // load the program to the shader
  glUseProgram(shaderProg);

  viewMat.m = (float *) viewMat.vm;
  projMat.m = (float *) projMat.vm;

  // transfer the matrix to the shader
  GLuint locMat= 0;
  locMat=glGetUniformLocation(shaderProg,  "modelViewProjMat");
  glUniformMatrix4fv(locMat,1,1,(float *)m.vm);

  m = viewMat*modelMat;
  locMat=glGetUniformLocation(shaderProg,  "modelViewMat");
  glUniformMatrix4fv(locMat,1,1,(float *) m.vm);

  // set the time
  GLuint tLoc = glGetUniformLocation(shaderProg, "t");
  glUniform1i(tLoc, t);

  // set the time
  GLuint cameraPositiontLoc = glGetUniformLocation(shaderProg, "cameraPosition");
  glEnableVertexAttribArray(cameraPositiontLoc);

  // Vector3f camPos = cam.getPosition();
  glUniform3f(cameraPositiontLoc, camPos.x, camPos.y, camPos.z);

  //glActiveTexture (GL_TEXTURE1);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texHandle);
  GLuint texLoc = glGetUniformLocation(shaderProg, "texCube");
    glUniform1i(texLoc, 3);
  GLint ttt = 0;
  glGetUniformiv(shaderProg, texLoc, &ttt);

  //glUniform1i(glGetUniformLocation(skyboxProg, "texCube"), texCube);
  //glDisable(GL_CULL_FACE);

  // bind the buffers to the shaders
  GLuint positionLoc = glGetAttribLocation(shaderProg, "vertex_position");
  //GLuint normalLoc = glGetAttribLocation(shaderProg, "vertex_normal");
  //GLuint texLoc = glGetAttribLocation(shaderProg, "texCoord");

  glEnableVertexAttribArray(positionLoc);
  //glEnableVertexAttribArray(normalLoc);
  //glEnableVertexAttribArray(texLoc);
  glBindBuffer(GL_ARRAY_BUFFER, vboCube);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCube);


  // Tells OpenGL how to walk through the two VBOs
  glVertexAttribPointer(positionLoc,4,GL_FLOAT, GL_FALSE, 0,0);
  // draw the triangles
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (char*) NULL+0);

  //glutSwapBuffers();
  glEnable(GL_DEPTH_TEST);    // need depth test to correctly draw 3D objects
  return;
}

GLuint SkyBox::getTexHandle(void) {
  return (texHandle);
}

/* this function creates a cube aound (0,0,0).

Ouptut:
vboCube - the vertex buffer offer of the cube
iboCube - the index buffer offer of the cube
Return:
the function returns 0 is successful.
*/
int SkyBox::createCube(void) {
  float vtx1[8][4] = {
      {-1.0,  1.0,  1.0, 1.0},
      {-1.0, -1.0,  1.0, 1.0},
      {1.0, -1.0,  1.0, 1.0},
      {1.0,  1.0,  1.0, 1.0},
      {-1.0,  1.0, -1.0, 1.0},
      {-1.0, -1.0, -1.0, 1.0},
      {1.0, -1.0, -1.0, 1.0},
      {1.0,  1.0, -1.0, 1.0}};

  float vtx[8][4] = {
      {-10.0,  10.0,  10.0, 1.0},
      {-10.0, -10.0,  10.0, 1.0},
      {10.0, -10.0,  10.0, 1.0},
      {10.0,  10.0,  10.0, 1.0},
      {-10.0,  10.0, -10.0, 1.0},
      {-10.0, -10.0, -10.0, 1.0},
      {10.0, -10.0, -10.0, 1.0},
      {10.0,  10.0, -10.0, 1.0}};

  glGenBuffers(1, &vboCube);
  printf("sizeof vtx=%d \n",sizeof(vtx));
  glBindBuffer(GL_ARRAY_BUFFER, vboCube);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vtx), vtx, GL_STATIC_DRAW);
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint ind[12][3] = {
    {0, 1, 2},
    {2, 3, 0},
    {3, 2, 6},
    {6, 7, 3},
    {7, 6, 5},
    {5, 4, 7},
    {4, 5, 1},
    {1, 0, 4},
    {0, 3, 7},
    {7, 4, 0},
    {1, 2, 6},
    {6, 5, 1},
  };

  printf("sizeof ind=%d size of gluint*36=%d \n",sizeof(ind), sizeof(GLuint)*36);
  glGenBuffers(1, &iboCube);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCube);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

  return 0;
}

/*******************************************************************************/

int SkyBox::init(void) {
  int rc;
  rc = s.createShaderProgram("skybox.vert","skybox.frag", &shaderProg);
  if (rc != 0) {
    printf(" error after generating skybox shaders \n");
    getchar();
    exit(1);
  }

  createCube();

  return 0;
}

// loads a colour textures
// front (posZ) is purple (magenta),
// back (negZ) is yellow,
// left (negX) is green
// right (posX) is red
// top (posY) is blue)
// bottom (negY) is cyan
int SkyBox::loadColourTexture(void) {
  int width=1, height=1;
  GLubyte posX[4] = {255, 0, 0, 1};
  GLubyte negX[4] = {0, 255, 0, 1};
  GLubyte posY[4] = {0, 0, 255, 1};
  GLubyte negY[4] = {0, 255, 255, 1};
  GLubyte posZ[4] = {255, 0, 255, 1};
  GLubyte negZ[4] = {255, 255, 0, 1};

  // Create texture object
  glGenTextures(1, &texHandle);
  //glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texHandle);
  checkError();

  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, posX);
  checkError();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, negX);
  checkError();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, posY);
  checkError();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, negY);
  checkError();
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, posZ);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE, negZ);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
  checkError();
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
  checkError();

  return 0;
}
