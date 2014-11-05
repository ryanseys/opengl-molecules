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
const GLfloat ATOM_RADIUS = 0.9;

Vector3f position (10, 10, 10);
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

  /**
   * Go through each atom and draw it.
   */
  for(std::vector<Atom>::iterator atom = atom_list.begin(); atom != atom_list.end(); ++atom) {
    atom->applyTransformation(worldMat);
    atom->draw(shaderProg);
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
  pugi::xml_document doc;
  if (!doc.load_file("ethyl.cml")) return -1;
  std::string name = doc.child("molecule").child_value("name");
  std::cout << "Loaded molecule: " << doc.child("molecule").child_value("name") << std::endl;

  Shader s;
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(800, 600);

  if(name.length() > 0) {
    glutCreateWindow(name.c_str());
  } else {
    glutCreateWindow("OpenGL Molecules");
  }

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboardFunc);
  glutSpecialFunc(pressSpecialKey);

  s.createShaderProgram("sphere.vert", "sphere.frag", &shaderProg);

  cam = new Camera(position, lookAtPoint, upVector);



  pugi::xml_node atoms = doc.child("molecule").child("atomArray");
  for (pugi::xml_node atom = atoms.child("atom"); atom; atom = atom.next_sibling("atom")) {
    GLfloat x = atom.attribute("x3").as_float();
    GLfloat y = atom.attribute("y3").as_float();
    GLfloat z = atom.attribute("z3").as_float();

    std::cout << "Element: " << atom.attribute("elementType").value();
    std::cout << ", X: " << x;
    std::cout << ", Y: " << y;
    std::cout << ", Z: " << z;
    std::cout << std::endl;

    atom_list.push_back(Atom(ATOM_RADIUS, x, y, z));
  }

  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
