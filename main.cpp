#include <stdio.h>
#include <cmath>
#include "math.h"
#include <iostream>
#include "pugixml.hpp"
#include "Shader.h"
#include "ryan_atom.h"
#include "ryan_cube.h"
#include "ryan_camera.h"
#include "ryan_matrix.h"

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 1.0; // degrees right and left
const GLfloat FORWARD_AMT = 10;
const GLfloat TIMER_TICK = 20; // milliseconds
const GLfloat ATOM_RADIUS = 1;

Vector3f position (100, 100, 100);
Vector3f lookAtPoint(0, 0, 0);
Vector3f upVector(0, 1, 0);

Camera * cam;
GLuint shaderProg;
GLint windowHeight, windowWidth;
std::vector<Atom> atom_list;

void display() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0,1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // setting up the transformaiton of the object from model coord. system to world coord.
  Matrix4f worldMat = cam->getViewMatrix();

  glUseProgram(shaderProg);

  // sphere->applyTransformation(worldMat);

  // draw them spheres, applying all transformations
  // sphere->drawSphere(shaderProg);
  //
  for(std::vector<Atom>::iterator it = atom_list.begin(); it != atom_list.end(); ++it) {
    it->applyTransformation(worldMat);
    it->draw(shaderProg);
  }

  glUseProgram(0);
  glFlush();
  glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
  cam->reshape(w, h);
}

void renderTick(int value) {
  // do stuff here
  glutPostRedisplay();
  glutTimerFunc(TIMER_TICK, renderTick, 1); // restart the timer
}

void keyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
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

  s.createShaderProgram("sphere.vert", "sphere.frag", &shaderProg);

  // For Task 1.
  // sphere = new SolidSphere(0.75, 24, 24);

  cam = new Camera(position, lookAtPoint, upVector);

  pugi::xml_document doc;
  if (!doc.load_file("caffeine.cml")) return -1;
  std::cout << "Loaded molecule: " << doc.child("molecule").child_value("name") << std::endl;
  pugi::xml_node atoms = doc.child("molecule").child("atomArray");
  for (pugi::xml_node atom = atoms.child("atom"); atom; atom = atom.next_sibling("atom")) {
    std::cout << "Element: " << atom.attribute("elementType").value();
    std::cout << ", X: " << atom.attribute("x3").as_float();
    std::cout << ", Y: " << atom.attribute("y3").as_float();
    std::cout << ", Z: " << atom.attribute("z3").as_float();
    std::cout << std::endl;

    GLfloat x = atom.attribute("x3").as_float();
    GLfloat y = atom.attribute("y3").as_float();
    GLfloat z = atom.attribute("z3").as_float();
    atom_list.push_back(Atom(ATOM_RADIUS, x, y, z));
  }

  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
