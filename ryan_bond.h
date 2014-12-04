#ifndef RYAN_BOND
#define RYAN_BOND

#include "ryan_matrix.h"
#include "cylinder.h"
#include "ryan_vector.h"
#include <iostream>
#include <cmath>

class Bond {
protected:
  Cylinder * firstBond;
  Cylinder * secondBond;
  Cylinder * thirdBond;
  GLfloat x, y, z;
  Atom firstAtom;
  Atom secondAtom;
  int numBonds = 0;
public:
  Bond(Atom firstAtom, Atom secondAtom, int order) {
    this->numBonds = order;
    this->firstAtom = firstAtom;
    this->secondAtom = secondAtom;
    this->firstBond = new Cylinder(30);
    this->secondBond = new Cylinder(30);
    this->thirdBond = new Cylinder(30);
    this->x = (secondAtom.x + firstAtom.x) / 2.0;
    this->y = (secondAtom.y + firstAtom.y) / 2.0;
    this->z = (secondAtom.z + firstAtom.z) / 2.0;
  }

  ~Bond() {
    // destructor
  }

  void draw(GLuint shaderProg, GLfloat rotateAngle) {
    this->firstBond->setAmbient(0.5, 0.5, 0.5);
    this->firstBond->setDiffuse(0.4, 0.4, 0.4);
    this->firstBond->setSpecular(0.1, 0.1, 0.1);

    if(numBonds >= 2) {
      this->secondBond->setAmbient(0.5, 0.5, 0.5);
      this->secondBond->setDiffuse(0.4, 0.4, 0.4);
      this->secondBond->setSpecular(0.1, 0.1, 0.1);
    }

    if(numBonds >= 3) {
      this->thirdBond->setAmbient(0.5, 0.5, 0.5);
      this->thirdBond->setDiffuse(0.4, 0.4, 0.4);
      this->thirdBond->setSpecular(0.1, 0.1, 0.1);
    }

    this->translate(this->x, this->y, this->z);

    // Get diff between two points (line the firstBond should follow)
    Vector3f p(
      secondAtom.x - firstAtom.x,
      secondAtom.y - firstAtom.y,
      secondAtom.z - firstAtom.z
    );

    // This is the default direction for the firstBond
    Vector3f z(0, 1.0, 0);

    // Get cross product (the axis of rotation)
    Vector3f t = Vector3f::cross(z, Vector3f::normalize(p));

    // Get angle.
    double angle = 180.0 / M_PI * acos(Vector3f::dot(z, p) / p.length());

    Vector3f trans = Vector3f::normalize(t);

    if(numBonds == 1) {
      this->firstBond->rotateVector(t, angle);
      this->firstBond->scale(0.17, p.length() * 2 * 0.17, 0.17);
      this->firstBond->draw(shaderProg);
    }
    else if(numBonds == 2) {
      this->firstBond->translate(trans.x*0.15, trans.y*0.15, trans.z*0.15);
      this->firstBond->rotateVector(t, angle);
      this->firstBond->scale(0.10, p.length() * 2 * 0.17, 0.10);
      this->firstBond->draw(shaderProg);

      this->secondBond->translate(-trans.x*0.15, -trans.y*0.15, -trans.z*0.15);
      this->secondBond->rotateVector(t, angle);
      this->secondBond->scale(0.10, p.length() * 2 * 0.17, 0.10);
      this->secondBond->draw(shaderProg);
    } else if(numBonds == 3) {
      this->firstBond->translate(trans.x*0.2, trans.y*0.2, trans.z*0.2);
      this->firstBond->rotateVector(t, angle);
      this->firstBond->scale(0.09, p.length() * 2 * 0.17, 0.09);
      this->firstBond->draw(shaderProg);

      this->secondBond->rotateVector(t, angle);
      this->secondBond->scale(0.09, p.length() * 2 * 0.17, 0.09);
      this->secondBond->draw(shaderProg);

      this->thirdBond->translate(-trans.x*0.2, -trans.y*0.2, -trans.z*0.2);
      this->thirdBond->rotateVector(t, angle);
      this->thirdBond->scale(0.09, p.length() * 2 * 0.17, 0.09);
      this->thirdBond->draw(shaderProg);
    }
  }

  void rotateX(GLfloat angle, int degrees) {
    this->firstBond->rotateX(angle, degrees);
    this->secondBond->rotateX(angle, degrees);
    this->thirdBond->rotateX(angle, degrees);
  }

  void rotateY(GLfloat angle, int degrees) {
    this->firstBond->rotateY(angle, degrees);
    this->secondBond->rotateY(angle, degrees);
    this->thirdBond->rotateY(angle, degrees);
  }

  void rotateZ(GLfloat angle, int degrees) {
    this->firstBond->rotateZ(angle, degrees);
    this->secondBond->rotateZ(angle, degrees);
    this->thirdBond->rotateZ(angle, degrees);
  }

  void rotateVector(Vector3f v, GLfloat angle, int degrees) {
    this->firstBond->rotateVector(v, angle, degrees);
    this->secondBond->rotateVector(v, angle, degrees);
    this->thirdBond->rotateVector(v, angle, degrees);
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    this->firstBond->translate(x, y, z);
    this->secondBond->translate(x, y, z);
    this->thirdBond->translate(x, y, z);
  }

  void clear() {
    this->firstBond->clear();
    this->secondBond->clear();
    this->thirdBond->clear();
  }
};

#endif
