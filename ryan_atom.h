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
    this->applyTransformation(Matrix4f::translation(this->x, this->y, this->z));
  }

  ~Atom() {
    // destructor
  }

  void draw(GLuint shaderProg) {
    this->applyTransformation(Matrix4f::translation(this->x, this->y, this->z));
    this->sphere->drawSphere(shaderProg);
  }

  void applyTransformation(Matrix4f m) {
    this->sphere->applyTransformation(m);
  }

  void clear() {
    this->sphere->clear();
  }
};

#endif
