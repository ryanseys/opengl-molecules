#ifndef RYAN_ATOM
#define RYAN_ATOM

#include "ryan_matrix.h"
#include "ryan_sphere.h"

class Atom {
protected:
  SolidSphere * sphere;
  GLfloat x, y, z, radius;
public:
  Atom(GLfloat radius, GLfloat x, GLfloat y, GLfloat z, char type) {
    sphere = new SolidSphere(radius, 30, 30);
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->z = z;
    switch(type) {
      case 'H':
        this->sphere->setAmbient(0.5, 0.5, 0.5); // material ambient color
        this->sphere->setDiffuse(0.5, 0.5, 0.5); // material diffuse color
        this->sphere->setSpecular(0.1, 0.1, 0.1); // material specular color
        break;
      default:
        this->sphere->setAmbient(1.0, 0.0, 0.2); // material ambient color
        this->sphere->setDiffuse(1.0, 1.0, 0.5); // material diffuse color
        this->sphere->setSpecular(0.8, 0.8, 0.8); // material specular color
        break;
    }
  }

  ~Atom() {
    // destructor
  }

  void draw(GLuint shaderProg) {
    this->translate(this->x, this->y, this->z);
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
