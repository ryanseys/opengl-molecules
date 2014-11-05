/**
 * COMP 4002 - Assignment 2
 *
 * Task 1: Render a sphere at (100, 10, 100) using perspective projection.
 * Task 2: Render a hierarchical object beside the sphere from Task 1.
 * Task 3: Create a camera class with yaw, pitch and roll.
 * Task 4: Bonus: Use keys 1-5 to select robot arm, then z & x to move arm piece.
 *
 * Author: Ryan Seys - 100817604
 */

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <cmath>
#include "math.h"
#include "Shader.h"
#include "ryan_sphere.h"
#include "ryan_cube.h"
#include "ryan_camera.h"
#include "ryan_matrix.h"
#include "ryan_robotarm.h"

GLdouble delta = 0.1; // how much to move the object (Task 2)

GLdouble objX = 0.0;
GLdouble objY = 0.0;
GLdouble objZ = 0.0;

GLint robotPartSelected = -1; // nothing initially selected
GLfloat ROBOT_ROTATE_DEG = 1.0;

GLuint shaderProg;
GLint windowHeight, windowWidth;

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 1.0; // degrees right and left
const GLfloat FORWARD_AMT = 0.2;

Vector3f position (106, 16, 106);
Vector3f lookAtPoint(100, 10, 100);
Vector3f upVector(0, 1, 0);

// initialize camera
Camera * cam;

GLdouble rotateDelta1 = 0.1; // Rotate first sphere 0.1 degrees per frame
GLdouble rotateDelta2 = 0.2; // Rotate second sphere 0.2 degrees per frame
GLdouble sphere1Rotate = 0.0;
GLdouble sphere2Rotate = 0.0;
GLint timerMs = 1;

// Robot arm
RobotArm * robotarm;
SolidSphere * sphere0;
SolidSphere * sphere1;
SolidSphere * sphere2;
SolidCube * cube;

/**
 * When a regular (not special) key is pressed.
 */
void keyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
    case '1': {
      robotPartSelected = 1;
      break;
    }
    case '2': {
      robotPartSelected = 2;
      break;
    }
    case '3': {
      robotPartSelected = 3;
      break;
    }
    case '4': {
      robotPartSelected = 4;
      break;
    }
    case '5': {
      robotPartSelected = 5;
      break;
    }
    case 'i': {
      objX -= delta;
      break;
    }
    case 'j': {
      objZ -= delta;
      break;
    }
    case 'k': {
      objZ += delta;
      break;
    }
    case 'l': {
      objX += delta;
      break;
    }
    case 'a': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. -PITCH_AMT vs. PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam->roll(-PITCH_AMT);
      break;
    }
    case 'd': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. PITCH_AMT vs. -PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam->roll(PITCH_AMT);
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
    case 'z': {
      // Rotate robot part +1 degree
      robotarm->rotatePart(robotPartSelected, ROBOT_ROTATE_DEG);
      break;
    }
    case 'x': {
      // Rotate robot part -1 degree
      robotarm->rotatePart(robotPartSelected, -ROBOT_ROTATE_DEG);
      break;
    }
    default: return;
  }
  glutPostRedisplay();
}

/**
 * Rendering the window.
 */
void display() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  Matrix4f initTranslateMat = Matrix4f::translation(100, 10, 100);

  // setting up the transformaiton of the object from model coord. system to world coord.
  Matrix4f worldMat = cam->getViewMatrix() * initTranslateMat;

  glUseProgram(shaderProg);

  sphere0->applyTransformation(worldMat);
  Matrix4f objMat = Matrix4f::translation(objX, objY, objZ);

  sphere1->applyTransformation(worldMat);
  sphere1->translate(-0.7, 1.0, -1.25);
  sphere1->applyTransformation(objMat);
  sphere1->rotateY(sphere1Rotate);

  sphere2->applyTransformation(worldMat);
  sphere2->translate(0.7, 1.0, -1.25);
  sphere2->applyTransformation(objMat);
  sphere2->rotateY(sphere2Rotate);

  cube->applyTransformation(worldMat);
  cube->applyTransformation(objMat);
  cube->translate(-0.25, -0.25, -1.6);

  robotarm->applyTransformation(worldMat);
  robotarm->applyTransformation(Matrix4f::translation(-3, 0.0, 1.0));

  // draw them spheres, applying all transformations
  sphere0->drawSphere(shaderProg);
  sphere1->drawSphere(shaderProg);
  sphere2->drawSphere(shaderProg);
  cube->draw(shaderProg);
  robotarm->draw(shaderProg);

  glUseProgram(0);
  glFlush();
  glutSwapBuffers();
}

/**
 * When the window reshapes to a new size, you must update the camera.
 *
 * @param w the window new width
 * @param h the window new height
 */
void reshape(GLint w, GLint h) {
  cam->reshape(w, h);
}

/**
 * Every time the timer ticks
 */
void renderTick(int value) {
  sphere1Rotate = fmod(sphere1Rotate + rotateDelta1, 360);
  sphere2Rotate = fmod(sphere2Rotate - rotateDelta2, 360);
  glutPostRedisplay();
  glutTimerFunc(timerMs, renderTick, 1); // restart the timer
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
      cam->yaw(YAW_AMT);
      break;
    }
    case GLUT_KEY_LEFT: {
      cam->yaw(-YAW_AMT);
      break;
    }
  }
  glutPostRedisplay();
}

/**
 * Main.
 */
int main(int argc, char** argv) {
  Shader s;
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);
  glutCreateWindow("COMP 4002 - Assignment 2 - Ryan Seys");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(pressSpecialKey);

  s.createShaderProgram("sphere.vert", "sphere.frag", &shaderProg);

  // For Task 1.
  sphere0 = new SolidSphere(0.75, 24, 24);

  // Object for Task 2.
  cube = new SolidCube(1.0, 0.5, 0.5);
  sphere1 = new SolidSphere(0.75, 24, 24);
  sphere2 = new SolidSphere(0.75, 24, 24);

  // For Task 3.
  cam = new Camera(position, lookAtPoint, upVector);

  // Robot arm for Task 4 (Bonus)
  robotarm = new RobotArm();

  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
