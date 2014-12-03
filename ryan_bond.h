#ifndef RYAN_BOND
#define RYAN_BOND

#include "ryan_matrix.h"
#include "cylinder.h"
#include "ryan_vector.h"
#include <iostream>
#include <cmath>

class Bond {
protected:
  Cylinder * cylinder;
  GLfloat x, y, z;
  Atom firstAtom;
  Atom secondAtom;
public:
  Bond(Atom firstAtom, Atom secondAtom) {
    this->firstAtom = firstAtom;
    this->secondAtom = secondAtom;
    this->cylinder = new Cylinder(30);
    this->x = (secondAtom.x + firstAtom.x) / 2.0;
    this->y = (secondAtom.y + firstAtom.y) / 2.0;
    this->z = (secondAtom.z + firstAtom.z) / 2.0;
  }

  ~Bond() {
    // destructor
  }

  void draw(GLuint shaderProg, GLfloat rotateAngle) {
    this->cylinder->setAmbient(0.5, 0.5, 0.5);
    this->cylinder->setDiffuse(0.4, 0.4, 0.4);
    this->cylinder->setSpecular(0.1, 0.1, 0.1);

    this->translate(this->x, this->y, this->z);

    // Get diff between two points (line the cylinder should follow)
    Vector3f p(
      secondAtom.x - firstAtom.x,
      secondAtom.y - firstAtom.y,
      secondAtom.z - firstAtom.z
    );

    // This is the default direction for the cylinder
    Vector3f z(0, 1.0, 0);

    // Get cross product (the axis of rotation)
    Vector3f t = Vector3f::cross(z, Vector3f::normalize(p));

    // Get angle.
    double angle = 180.0 / M_PI * acos(Vector3f::dot(z, p) / p.length());

    // printf("Angle: %f\n", angle);
    this->cylinder->rotateVector(t, angle);
    this->cylinder->scale(0.17, p.length() * 2 * 0.17, 0.17);
    this->cylinder->draw(shaderProg);
  }

  void rotateY(GLfloat angle, int degrees) {
    this->cylinder->rotateY(angle, degrees);
  }

  void rotateX(GLfloat angle, int degrees) {
    this->cylinder->rotateX(angle, degrees);
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    this->cylinder->translate(x, y, z);
  }

  void clear() {
    this->cylinder->clear();
  }
};

#endif
