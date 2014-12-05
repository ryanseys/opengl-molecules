#include "SkyBox.h"
#include "stdio.h"
#include "stdlib.h"
#include "SOIL.h"
#include "ryan_matrix.h"

static int checkError() {
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

  // set up the mode to wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //set up the mode to fill
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  Vector3f camPos = cam.getPosition();

  // get the transformation matrix
  modelMat = Matrix4f::translation(camPos.x, camPos.y, camPos.z);

  // setting up the viewpoint transformation
  viewMat = cam.getViewMatrix();
  projMat = cam.getProjMatrix();

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

  // bind the buffers to the shaders
  GLuint positionLoc = glGetAttribLocation(shaderProg, "vertex_position");

  glEnableVertexAttribArray(positionLoc);
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
  glBindBuffer(GL_ARRAY_BUFFER, vboCube);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vtx), vtx, GL_STATIC_DRAW);

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

  glGenBuffers(1, &iboCube);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboCube);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);

  return 0;
}

/**
 * Initialize the Skybox.
 */
int SkyBox::init(void) {
  int rc;
  rc = s.createShaderProgram("skybox.vert","skybox.frag", &shaderProg);
  if (rc != 0) {
    printf("Error after generating skybox shaders.\n");
    getchar();
    exit(1);
  }

  createCube();
  return 0;
}
