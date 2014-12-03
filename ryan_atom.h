#ifndef RYAN_ATOM
#define RYAN_ATOM

// #include "ryan_matrix.h"
#include "ryan_sphere.h"

class Atom {
protected:
  SolidSphere * sphere;
  GLfloat radius;
public:
  GLfloat x, y, z;
  Atom() {}
  Atom(GLfloat radius, GLfloat x, GLfloat y, GLfloat z, char type) {
    sphere = new SolidSphere(radius, 60, 60);
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->z = z;
    switch(type) {
      case 'H':
        // hydrogen is white but not too white (so we make it light grey)
        this->sphere->setAmbient(0.5, 0.5, 0.5); // material ambient color
        this->sphere->setDiffuse(0.4, 0.4, 0.4); // material diffuse color
        this->sphere->setSpecular(0.1, 0.1, 0.1); // material specular color
        this->sphere->setRadius(0.5); // hydrogen is smaller
        break;
      case 'C':
        // carbon is black but not too black (so we make it dark grey)
        this->sphere->setAmbient(0, 0, 0); // material ambient color
        this->sphere->setDiffuse(0.2, 0.2, 0.2); // material diffuse color
        this->sphere->setSpecular(0.1, 0.1, 0.1); // material specular color
        break;
      case 'O':
        // oxygen is red
        this->sphere->setAmbient(1.0, 0.15, 0.15); // material ambient color
        this->sphere->setDiffuse(1.0, 0.15, 0.15); // material diffuse color
        this->sphere->setSpecular(1.0, 0.15, 0.15); // material specular color
        break;
      case 'N':
        // nitrogen is blue
        this->sphere->setAmbient(0.1, 0.1, 1.0); // material ambient color
        this->sphere->setDiffuse(0.1, 0.1, 1.0); // material diffuse color
        this->sphere->setSpecular(0.1, 0.1, 1.0); // material specular color
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

  void setRadius(GLfloat r) {
    this->radius = r;
  }

  float getRadius() {
    return radius;
  }

  void rotateX(GLfloat angle, int degrees) {
    this->sphere->rotateX(angle, degrees);
  }

  void rotateY(GLfloat angle, int degrees) {
    this->sphere->rotateY(angle, degrees);
  }

  void draw(GLuint shaderProg) {
    this->translate(this->x, this->y, this->z);
    this->sphere->draw(shaderProg);
  }

  void translate(GLfloat x, GLfloat y, GLfloat z) {
    this->sphere->translate(x, y, z);
  }

  void clear() {
    this->sphere->clear();
  }
};

#endif
