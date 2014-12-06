#include <stdio.h>
#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>
#include <map>
#include "Shader.h"
#include "ryan_camera.h"
#include "ryan_matrix.h"
#include "ryan_light.h"
#include "ryan_molecule.h"
#include "SOIL.h"
#include "SkyBox.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 5.0; // degrees right and left
const GLfloat FORWARD_AMT = 0.5;
const GLfloat TIMER_TICK = 20; // milliseconds

Vector3f position (0.0, 1.0, 10.0);
Vector3f lookAtPoint(0, 0, 0);
Vector3f upVector(0, 1, 0);

int xStart, yStart;
int xLast, yLast;
int valid = 0;
int moleculeFileIndex = 0;

std::vector<std::string> files;

Molecule * molecule;
GLfloat rotateMoleculeX = 0;
GLfloat rotateMoleculeY = 0;

Camera * cam;

int isPaused = 0;

// Skybox related variables
SkyBox skybox;
GLuint skyboxProg;

Light * light;
Light * spotlight;

GLfloat shininess = 5.0; // min is zero
GLfloat SHINY_FACTOR = 5.0;

GLuint shaderProg;
GLint windowHeight, windowWidth;

GLfloat angularAtten = 55;
GLfloat coneAngle = 40;

void getMoleculeFiles() {
  std::ifstream moleculeListFile("molecules_list.txt");
  std::string line;
  while (std::getline(moleculeListFile, line)) {
    files.push_back(line);
  }
}

void display() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  skybox.displaySkybox(*cam);
  glUseProgram(shaderProg);

  GLuint viewMatLoc = glGetUniformLocation(shaderProg,  "viewMat");
  glUniformMatrix4fv(viewMatLoc, 1, 1, (float *) cam->getViewMatrix().vm);

  GLuint projMatLoc = glGetUniformLocation(shaderProg,  "projMat");
  glUniformMatrix4fv(projMatLoc, 1, 1, (float *) cam->getProjMatrix().vm);

  GLuint lightAmbLoc = glGetUniformLocation(shaderProg,  "lightAmb");
  glUniform4fv(lightAmbLoc, 1, (float *) &light->ambient);

  GLuint lightDiffLoc = glGetUniformLocation(shaderProg,  "lightDiff");
  glUniform4fv(lightDiffLoc, 1, (float *) &light->diffuse);

  GLuint lightSpecLoc = glGetUniformLocation(shaderProg,  "lightSpec");
  glUniform4fv(lightSpecLoc, 1, (float *) &light->specular);

  GLuint lightPosLoc = glGetUniformLocation(shaderProg,  "lightPos");
  glUniform4fv(lightPosLoc, 1, (float *) &light->position);

  GLuint shininessLoc = glGetUniformLocation(shaderProg,  "shininess");
  glUniform1f(shininessLoc, shininess);

  GLuint spotPosLoc = glGetUniformLocation(shaderProg,  "spotPos");
  glUniform4fv(spotPosLoc, 1, (float *) &spotlight->position);

  GLuint spotLookAtLoc = glGetUniformLocation(shaderProg,  "spotLookAtPnt");
  glUniform4fv(spotLookAtLoc, 1, (float *) &spotlight->lookAtPoint);

  GLuint spotAngAttenLoc = glGetUniformLocation(shaderProg,  "spotAngAtten");
  glUniform1f(spotAngAttenLoc, spotlight->angularAtten);

  GLuint spotConeAngleLoc = glGetUniformLocation(shaderProg,  "spotConeAngle");
  glUniform1f(spotConeAngleLoc, spotlight->coneAngle);

  glActiveTexture(GL_TEXTURE3);
  GLuint texCube = skybox.getTexHandle();
  glBindTexture(GL_TEXTURE_CUBE_MAP, texCube);
  GLuint samLoc = glGetUniformLocation(shaderProg, "texCube");
  glUniform1i(samLoc, 3);

  GLint ttt = 0;
  glGetUniformiv(shaderProg, samLoc, &ttt);

  GLuint camPosLoc = glGetUniformLocation(shaderProg,  "camPos");
  glUniform4fv(camPosLoc, 1, (float *) &cam->position);

  molecule->rotateX(rotateMoleculeX);
  molecule->rotateY(rotateMoleculeY);
  molecule->draw(shaderProg);

  glUseProgram(0);

  glFlush();
  glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  cam->reshape(w, h);
}

void renderTick(int value) {
  if(!isPaused){
    rotateMoleculeY = (rotateMoleculeY + 0.006);
  }
  glutPostRedisplay();
  glutTimerFunc(TIMER_TICK, renderTick, 1); // restart the timer
}

void keyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
    case 'a': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. -PITCH_AMT vs. PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam->yaw(-YAW_AMT);
      break;
    }
    case 'd': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. PITCH_AMT vs. -PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam->yaw(YAW_AMT);
      break;
    }
    case 'w': {
      // Move camera forward along lookAtVector
      cam->moveForward(FORWARD_AMT);
      break;
    }
    case 's': {
      // Move camera backward along lookAtVector
      cam->moveForward(-FORWARD_AMT);
      break;
    }
    case 'p': {
      // Pause the rotation
      isPaused = isPaused == 0 ? 1 : 0;
      printf("Play/pause toggled\n");
      break;
    }
    default: return;
  }
  glutPostRedisplay();
}

void pressSpecialKey(int key, int xx, int yy) {
  switch (key) {
    case GLUT_KEY_UP: {
      cam->pitch(PITCH_AMT);
      break;
    }
    case GLUT_KEY_DOWN: {
      cam->pitch(-PITCH_AMT);
      break;
    }
    case GLUT_KEY_RIGHT: {
      // next molecule
      moleculeFileIndex++;
      if(moleculeFileIndex >= files.size()) {
        moleculeFileIndex = 0;
      }
      molecule = new Molecule(files.at(moleculeFileIndex));
      break;
    }
    case GLUT_KEY_LEFT: {
      // previous molecule
      moleculeFileIndex--;
      if(moleculeFileIndex < 0) {
        moleculeFileIndex = files.size()-1;
      }
      molecule = new Molecule(files.at(moleculeFileIndex));
      break;
    }
  }
  glutPostRedisplay();
}

void mouseButton(int button, int state, int x, int y) {
  xLast = x;
  yLast = y;
  if(state == GLUT_DOWN) {
    valid = true;
    xStart = x;
    yStart = y;
  } else {
    valid = false;
  }
}

void mouseMove(int x, int y) {
  if (valid) {
    int dx = xLast - x;
    int dy = yLast - y;
    int totalX = xStart - x;
    int totalY = yStart - y;
    rotateMoleculeY -= (dx * 0.01);
    rotateMoleculeX -= (dy * 0.01);
  }
  xLast = x;
  yLast = y;
}

int main(int argc, char** argv) {
  Shader s;
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("OpenGL Molecules");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(pressSpecialKey);

  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);

  // load default molecule
  molecule = new Molecule("cmls/caffeine.cml");

  char *skyboxTex[6] ={
    "textures/bokeh_right.png",
    "textures/bokeh_left.png",
    "textures/bokeh_top.png",
    "textures/bokeh_bottom.png",
    "textures/bokeh_front.png",
    "textures/bokeh_back.png"
  };

  skybox.loadSkybox(skyboxTex);
  skybox.init();

  // Uncomment the code below to work on Windows!
  // GLenum err = glewInit();
  // if (err != GLEW_OK) {
  //   printf("Error initializing GLEW!\n");
  // }

  s.createShaderProgram("phong.vert", "phong.frag", &shaderProg);

  getMoleculeFiles();

  cam = new Camera(position, lookAtPoint, upVector);

  // Set up light
  light = new Light();

  // white light
  light->setAmbient(1.0, 1.0, 1.0);
  light->setDiffuse(1.0, 1.0, 1.0);
  light->setSpecular(1.0, 1.0, 1.0);
  light->setPosition(10, 10, 10);

  spotlight = new Light();
  spotlight->setAmbient(1.0, 1.0, 1.0);
  spotlight->setDiffuse(1.0, 1.0, 1.0);
  spotlight->setSpecular(1.0, 1.0, 1.0);
  spotlight->setPosition(10, 10, 10);
  spotlight->setAngularAttenuation(angularAtten);
  spotlight->setConeAngle(coneAngle);
  spotlight->setLookAtPoint(0, 0, 0);

  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
