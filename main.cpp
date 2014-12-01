#include <stdio.h>
#include <cmath>
#include "math.h"
#include <iostream>
#include <map>
#include "pugixml.hpp"
#include "Shader.h"
#include "ryan_atom.h"
#include "ryan_cube.h"
#include "ryan_camera.h"
#include "ryan_matrix.h"
#include "ryan_light.h"
#include "ryan_bond.h"

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 1.0; // degrees right and left
const GLfloat FORWARD_AMT = 2;
const GLfloat RIGHT_AMT = 0.5;
const GLfloat TIMER_TICK = 20; // milliseconds
const GLfloat ATOM_RADIUS = 0.9;

Vector3f position (10, 10, 10);
Vector3f lookAtPoint(0, 0, 0);
Vector3f upVector(0, 1, 0);

Camera * cam;

Light * light;
Light * spotlight;

GLfloat shininess = 5.0; // min is zero
GLfloat SHINY_FACTOR = 5.0;

GLuint shaderProg;
GLint windowHeight, windowWidth;
std::vector<Atom> atom_list;
std::vector<Bond> bond_list;

GLfloat angularAtten = 55;
GLfloat coneAngle = 40;

float addShininess(GLfloat amount) {
  GLfloat SHINY_MIN = 0;
  GLfloat SHINY_MAX = 250;
  shininess += amount;
  if(shininess < SHINY_MIN) {
    shininess = SHINY_MIN;
  } else if(shininess > SHINY_MAX) {
    shininess = SHINY_MAX;
  }
  return shininess;
}

void display() {
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

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

  // setting up the transformaiton of the object from model coord. system to world coord.
  Matrix4f worldMat = cam->getViewMatrix();

  /**
   * Go through each atom and draw it.
   */
  for(std::vector<Atom>::iterator atom = atom_list.begin(); atom != atom_list.end(); ++atom) {
    atom->draw(shaderProg);
  }

  // cyl->draw(shaderProg);

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
      cam->moveRight(-RIGHT_AMT);
      break;
    }
    case 'd': {
      // Roll camera counter-clockwise
      // Yes, this is backward (i.e. PITCH_AMT vs. -PITCH_AMT to the assignment
      // description but it makes more sense when using the keyboard controls.
      cam->moveRight(RIGHT_AMT);
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
    case 'x': {
      angularAtten++;
      spotlight->setAngularAttenuation(angularAtten);
      printf("angularAtten: %f\n", angularAtten);
      break;
    }
    case 'z': {
      angularAtten--;
      spotlight->setAngularAttenuation(angularAtten);
      printf("angularAtten: %f\n", angularAtten);
      break;
    }
    case 'v': {
      coneAngle++;
      spotlight->setConeAngle(coneAngle);
      printf("coneAngle: %f\n", coneAngle);
      break;
    }
    case 'c': {
      coneAngle--;
      spotlight->setConeAngle(coneAngle);
      printf("coneAngle: %f\n", coneAngle);
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
      // cam->pitch(-PITCH_AMT);
      break;
    }
    case GLUT_KEY_RIGHT: {
      cam->yaw(YAW_AMT);
      // cam->roll(PITCH_AMT);
      break;
    }
    case GLUT_KEY_LEFT: {
      cam->yaw(-YAW_AMT);
      // cam->roll(PITCH_AMT);

      break;
    }
  }
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  pugi::xml_document doc;
  if (!doc.load_file("caffeine.cml")) return -1;
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

  s.createShaderProgram("phong.vert", "phong.frag", &shaderProg);

  cam = new Camera(position, lookAtPoint, upVector);

  std::map<std::string, Atom> atom_map;

  // parse all atoms
  pugi::xml_node atoms = doc.child("molecule").child("atomArray");
  for (pugi::xml_node atom = atoms.child("atom"); atom; atom = atom.next_sibling("atom")) {
    GLfloat x = atom.attribute("x3").as_float();
    GLfloat y = atom.attribute("y3").as_float();
    GLfloat z = atom.attribute("z3").as_float();
    std::string id = atom.attribute("id").as_string();

    char atomType = *atom.attribute("elementType").value();

    std::cout << "Element: " << atom.attribute("elementType").value();
    std::cout << ", X: " << x;
    std::cout << ", Y: " << y;
    std::cout << ", Z: " << z;
    std::cout << std::endl;

    Atom tempAtom(ATOM_RADIUS, x, y, z, atomType);
    atom_map.insert(std::pair<std::string, Atom>(id, tempAtom));
    atom_list.push_back(tempAtom);
  }

  // parse all bonds
  pugi::xml_node bonds = doc.child("molecule").child("bondArray");
  for (pugi::xml_node bond = bonds.child("bond"); bond; bond = bond.next_sibling("bond")) {
    std::string refAtoms = bond.attribute("atomRefs2").as_string();

    // std::string s = "scott>=tiger>=mushroom";
    std::string delimiter = " ";

    int end = refAtoms.find(delimiter, 0);
    std::string token1 = refAtoms.substr(0, end);
    std::string token2 = refAtoms.substr(end+1, refAtoms.find(delimiter, refAtoms.length()-1));

    int order = bond.attribute("order").as_int();

    Bond tempBond(atom_map.find(token1)->second, atom_map.find(token2)->second);

    bond_list.push_back(tempBond);

    std::cout << "Bond: " << refAtoms;
    std::cout << ", order: " << order;
    std::cout << std::endl;
  }

  // Set up light
  light = new Light();

  // white light
  light->setAmbient(1.0, 1.0, 1.0);
  light->setDiffuse(1.0, 1.0, 1.0);
  light->setSpecular(1.0, 1.0, 1.0);
  light->setPosition(10, 10, 10);

  // Task 5: Create a spotlight
  spotlight = new Light();
  // white spotlight by default
  spotlight->setAmbient(1.0, 1.0, 1.0);
  spotlight->setDiffuse(1.0, 1.0, 1.0);
  spotlight->setSpecular(1.0, 1.0, 1.0);
  spotlight->setPosition(10, 10, 10);
  spotlight->setAngularAttenuation(angularAtten);
  spotlight->setConeAngle(coneAngle);
  spotlight->setLookAtPoint(0, 0, 0); // center of sphere

  glutPostRedisplay();
  glEnable(GL_DEPTH_TEST);
  glutTimerFunc(1, renderTick, 1);
  glutMainLoop();
  return 0;
}
