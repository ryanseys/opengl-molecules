#ifndef RYAN_BOND
#define RYAN_BOND

#include "ryan_matrix.h"
#include "cylinder.h"
#include <iostream>

class Bond {
protected:
  Cylinder * cyl;
  GLfloat x, y, z;
public:
  Bond(Atom a1, Atom a2) {
    this->cyl = new Cylinder(30);
    // this->x = x;
    // this->y = y;
    // this->z = z;
  }

  ~Bond() {
    // destructor
  }

  void draw(GLuint shaderProg) {
    this->translate(this->x, this->y, this->z);
    this->cyl->draw(shaderProg);
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    this->cyl->translate(x, y, z);
  }

  void clear() {
    this->cyl->clear();
  }
};

#endif
