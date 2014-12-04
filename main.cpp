#include <stdio.h>
#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>
#include <map>
#include "pugixml.hpp"
#include "Shader.h"
#include "ryan_atom.h"
#include "ryan_cube.h"
#include "ryan_camera.h"
#include "ryan_matrix.h"
#include "ryan_light.h"
#include "ryan_bond.h"
#include "SOIL.h"
#include "SkyBox.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

const GLfloat PITCH_AMT = 1.0; // degrees up and down
const GLfloat YAW_AMT = 1.0; // degrees right and left
const GLfloat FORWARD_AMT = 0.5;
const GLfloat RIGHT_AMT = 0.5;
const GLfloat TIMER_TICK = 20; // milliseconds
const GLfloat ATOM_RADIUS = 0.7;

Vector3f position (6.0, 6.0, 6.0);
Vector3f lookAtPoint(0, 0, 0);
Vector3f upVector(0, 1, 0);

GLfloat rotateAngle = 0;
GLfloat rotateMoleculeX = 0;
GLfloat rotateMoleculeY = 0;

int moleculeFileIndex = 0;
std::vector<std::string> files;

Camera * cam;

int isPaused = 0;

// Skybox related variables
SkyBox skybox;
GLuint skyboxProg;
GLuint sphereBoxProg;

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

void getMoleculeFiles() {
  std::ifstream moleculeListFile("molecules_list.txt");
  std::string line;
  while (std::getline(moleculeListFile, line)) {
    files.push_back(line);
  }
}

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

bool BothAreSpaces(char lhs, char rhs) {
  return (lhs == rhs) && (lhs == ' ');
}

std::string trim(std::string const& str) {
  std::size_t first = str.find_first_not_of(' ');
  std::size_t last  = str.find_last_not_of(' ');
  std::string newStr = str.substr(first, last-first+1);

  std::string::iterator new_end = std::unique(newStr.begin(), newStr.end(), BothAreSpaces);
  newStr.erase(new_end, newStr.end());
  return newStr;
}

void loadMolecule(std::string filename) {
  pugi::xml_document doc;
  if (!doc.load_file(filename.c_str())) {
    printf("Error: Could not load molecule file: %s\n", filename.c_str());
    std::exit(-1);
  }
  atom_list.clear();
  bond_list.clear();
  std::string name = doc.child("molecule").child_value("name");

  if(name.length() > 0) {
    glutSetWindowTitle(name.c_str());
  } else {
    glutSetWindowTitle(filename.c_str());
  }

  std::map<std::string, Atom> atom_map;

  // parse all atoms
  pugi::xml_node atoms = doc.child("molecule").child("atomArray");
  for (pugi::xml_node atom = atoms.child("atom"); atom; atom = atom.next_sibling("atom")) {
    GLfloat x = atom.attribute("x3").as_float();
    GLfloat y = atom.attribute("y3").as_float();
    GLfloat z = atom.attribute("z3").as_float();
    std::string id = atom.attribute("id").as_string();

    char atomType = *atom.attribute("elementType").value();

    // std::cout << "Element: " << atom.attribute("elementType").value();
    // std::cout << ", X: " << x;
    // std::cout << ", Y: " << y;
    // std::cout << ", Z: " << z;
    // std::cout << std::endl;

    Atom tempAtom(ATOM_RADIUS, x, y, z, atomType);
    atom_map.insert(std::pair<std::string, Atom>(id, tempAtom));
    atom_list.push_back(tempAtom);
  }

  // parse all bonds
  pugi::xml_node bonds = doc.child("molecule").child("bondArray");
  for (pugi::xml_node bond = bonds.child("bond"); bond; bond = bond.next_sibling("bond")) {
    std::string refAtoms = trim(bond.attribute("atomRefs2").as_string());

    std::string delimiter = " ";

    int end = refAtoms.find(delimiter, 0);
    std::string token1 = refAtoms.substr(0, end);
    std::string token2 = refAtoms.substr(end+1, refAtoms.find(delimiter, refAtoms.length()-1));

    int order = bond.attribute("order").as_int();

    Bond tempBond(atom_map.find(token1)->second, atom_map.find(token2)->second, order);

    bond_list.push_back(tempBond);

    // std::cout << "Bond: " << refAtoms;
    // std::cout << ", order: " << order;
    // std::cout << std::endl;
  }
  printf("Done loading molecule: %s\n", filename.c_str());
}

// Cylinder * cylinder;

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

  // setting up the transformation of the object from model coord. system to world coord.
  // Matrix4f worldMat = cam->getViewMatrix();

  // cylinder->translate(clickPos.x, clickPos.y, clickPos.z);
  // cylinder->rotateVector(cylinderVec, cylinderAngle);
  // cylinder->scale(0.01, 40, 0.01);
  // cylinder->draw(shaderProg);

  /**
   * Go through each atom and draw it.
   */
  for(std::vector<Atom>::iterator atom = atom_list.begin(); atom != atom_list.end(); ++atom) {
    atom->rotateY(rotateMoleculeY, 0);
    atom->rotateX(rotateMoleculeX, 0);
    atom->draw(shaderProg);
  }

  for(std::vector<Bond>::iterator bond = bond_list.begin(); bond != bond_list.end(); ++bond) {
    bond->rotateY(rotateMoleculeY, 0);
    bond->rotateX(rotateMoleculeX, 0);
    bond->draw(shaderProg, rotateAngle);
  }

  glUseProgram(0);

  glFlush();
  glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  cam->reshape(w, h);
}

int t = 0;
void idleFunc() {
  t++;
  glutPostRedisplay();
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
    case 'm': {
      rotateAngle+=0.1;
      printf("rotateAngle: %f\n", rotateAngle);
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
      loadMolecule(files.at(moleculeFileIndex));
      break;
    }
    case GLUT_KEY_LEFT: {
      // previous molecule
      moleculeFileIndex--;
      if(moleculeFileIndex < 0) {
        moleculeFileIndex = files.size()-1;
      }
      loadMolecule(files.at(moleculeFileIndex));
      break;
    }
  }
  glutPostRedisplay();
}

int xStart, yStart;
int xLast, yLast;
int valid = 0;

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
  glutIdleFunc(idleFunc);
  glutSpecialFunc(pressSpecialKey);

  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);

  // load molecule
  loadMolecule("cmls/caffeine.cml");

  char *skyboxTex[6] ={
    "textures/bokeh_right.png",
    "textures/bokeh_left.png",
    "textures/bokeh_top.png",
    "textures/bokeh_bottom.png",
    "textures/bokeh_front.png",
    "textures/bokeh_back.png"
  };

  // cylinder = new Cylinder(30);

  skybox.loadSkybox(skyboxTex);
  skybox.init();

  int rc = s.createShaderProgram("sphereBox.vert", "sphereBox.frag", &sphereBoxProg);
  if (rc != 0) {
    printf(" error after generating sphere skybox shaders \n");
    rc = 4;
  }

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

  // Task 5: Create a spotlight
  spotlight = new Light();
  // white spotlight by default
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
