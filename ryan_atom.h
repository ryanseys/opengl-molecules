#ifndef RYAN_ATOM
#define RYAN_ATOM

#include "ryan_matrix.h"
#include "ryan_sphere.h"

class Atom {
protected:
  SolidSphere * sphere;
  GLfloat x, y, z, radius;
public:
  Atom(GLfloat radius, GLfloat x, GLfloat y, GLfloat z) {
    sphere = new SolidSphere(radius, 30, 30);
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->z = z;
  }

  ~Atom() {
    // destructor
  }

  void setColors() {
    this->sphere->setAmbient(1.0, 0.0, 0.2); // material ambient color
    this->sphere->setDiffuse(1.0, 1.0, 0.5); // material diffuse color
    this->sphere->setSpecular(0.8, 0.8, 0.8); // material specular color
  }

  void draw(GLuint shaderProg) {
    this->translate(this->x, this->y, this->z);
    this->setColors();
    this->sphere->drawSphere(shaderProg);
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    this->sphere->translate(x, y, z);
  }

  void clear() {
    this->sphere->clear();
  }
};

#endif
