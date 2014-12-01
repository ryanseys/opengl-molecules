#ifndef RYAN_BOND
#define RYAN_BOND

#include "ryan_matrix.h"
#include "cylinder.h"
#include "ryan_vector.h"
#include <iostream>
#include <cmath>

class Bond {
protected:
  Cylinder * cyl;
  GLfloat x, y, z;
  Vector3f *direction;
  Atom * a1;
  Atom * a2;
public:
  Bond(Atom *a1, Atom *a2) {
    this->a1 = a1;
    this->a2 = a2;
    this->cyl = new Cylinder(30);
    this->x = (a2->x + a1->x)/2.0;
    this->y = (a2->y + a1->y)/2.0;
    this->z = (a2->z + a2->z)/2.0;
  }

  ~Bond() {
    // destructor
  }

  void draw(GLuint shaderProg, GLfloat rotateAngle) {
    this->cyl->setAmbient(0.5, 0.2, 0.3);
    this->cyl->setDiffuse(0.4, 0.5, 0.7);
    this->cyl->setSpecular(0.3, 0.4, 1.0);

    this->translate(this->x, this->y, this->z);

    // This is the default direction for the cylinder
    Vector3f z = Vector3f(0, 1, 0);
    // Get diff between two points (line the cylinder should follow)
    Vector3f p(a2->x - a1->x, a2->y - a1->y, a2->z - a1->z);
    // Get cross product (the axis of rotation)
    Vector3f t = Vector3f::cross(z, Vector3f::normalize(p));

    // Get angle.
    double angle = 180 / 3.14159 * acos(Vector3f::dot(z, p) / p.length());

    // printf("Angle: %f\n", angle);
    this->cyl->rotateVector(Vector3f(t.x, t.y, t.z), angle);

    this->cyl->scale(0.17, 0.17, 0.17);
    this->cyl->scale(1, p.length()*2, 1);
    this->cyl->draw(shaderProg);
  }

  void rotateY(GLfloat angle, int degrees) {
    this->cyl->rotateY(angle, degrees);
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    this->cyl->translate(x, y, z);
  }

  void clear() {
    this->cyl->clear();
  }
};

#endif
