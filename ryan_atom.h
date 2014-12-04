#ifndef RYAN_ATOM
#define RYAN_ATOM

#include "ryan_sphere.h"

class Atom {
protected:
  SolidSphere * sphere;
  GLfloat radius;
public:
  GLfloat x, y, z;
  Atom() {}
  Atom(GLfloat radius, GLfloat x, GLfloat y, GLfloat z, std::string type) {
    sphere = new SolidSphere(radius, 60, 60);
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->z = z;
    this->setColor(type);
  }

  ~Atom() {
  }

  GLfloat getDistance(Vector3f v) {
    return (Vector3f(x, y, z) - v).length();
  }

  void highlight() {
    this->sphere->setAmbient(1.0, 1.0, 1.0); // material ambient color
    this->sphere->setDiffuse(1.0, 1.0, 1.0); // material diffuse color
    this->sphere->setSpecular(1.0, 1.0, 1.0); // material specular color
  }

  Vector3f getPosition() {
    return Vector3f(x, y, z);
  }

  void setColor(std::string element) {
    if(element == "H") {
      // hydrogen is white but not too white (so we make it light grey)
      this->sphere->setAmbient(0.5, 0.5, 0.5); // material ambient color
      this->sphere->setDiffuse(0.4, 0.4, 0.4); // material diffuse color
      this->sphere->setSpecular(0.1, 0.1, 0.1); // material specular color
      this->sphere->setRadius(0.5); // hydrogen is smaller
    } else if(element == "C") {
      // carbon is black but not too black (so we make it dark grey)
      this->sphere->setAmbient(0, 0, 0); // material ambient color
      this->sphere->setDiffuse(0.2, 0.2, 0.2); // material diffuse color
      this->sphere->setSpecular(0.1, 0.1, 0.1); // material specular color
    } else if (element == "O") {
      // oxygen is red
      this->sphere->setAmbient(0.65, 0.15, 0.15); // material ambient color
      this->sphere->setDiffuse(0.65, 0.15, 0.15); // material diffuse color
      this->sphere->setSpecular(0.65, 0.15, 0.15); // material specular color
    } else if (element == "N") {
      // nitrogen is blue
      this->sphere->setAmbient(0.1, 0.1, 0.6); // material ambient color
      this->sphere->setDiffuse(0.1, 0.1, 0.6); // material diffuse color
      this->sphere->setSpecular(0.1, 0.1, 0.6); // material specular color
    } else if (element == "S") {
      // Sulfur is yellow
      this->sphere->setAmbient(0.6, 0.6, 0.1); // material ambient color
      this->sphere->setDiffuse(0.6, 0.6, 0.1); // material diffuse color
      this->sphere->setSpecular(0.6, 0.6, 0.1); // material specular color
    } else if(element == "F") {
      //fluorine is green
      this->sphere->setAmbient(0.1, 0.6, 0.1); // material ambient color
      this->sphere->setDiffuse(0.1, 0.6, 0.1); // material diffuse color
      this->sphere->setSpecular(0.1, 0.6, 0.1); // material specular color
    } else if(element == "Cl") {
      // chlorine is dark green
      this->sphere->setAmbient(0.1, 0.4, 0.1); // material ambient color
      this->sphere->setDiffuse(0.1, 0.4, 0.1); // material diffuse color
      this->sphere->setSpecular(0.1, 0.4, 0.1); // material specular color
    } else if (element == "Pt") {
      // platinum is light grey
      this->sphere->setAmbient(0.3, 0.3, 0.3); // material ambient color
      this->sphere->setDiffuse(0.5, 0.5, 0.5); // material diffuse color
      this->sphere->setSpecular(0.4, 0.4, 0.4); // material specular color
    } else if (element == "Fe") {
      // iron is brown-silver
      this->sphere->setAmbient(0.84, 0.39, 0.0); // material ambient color
      this->sphere->setDiffuse(0.84, 0.39, 0.0); // material diffuse color
      this->sphere->setSpecular(0.84, 0.39, 0.0); // material specular color
    } else if (element == "Du") {
      // iron is brown-silver
      this->sphere->setAmbient(0.84, 0.39, 0.0); // material ambient color
      this->sphere->setDiffuse(0.84, 0.39, 0.0); // material diffuse color
      this->sphere->setSpecular(0.84, 0.39, 0.0); // material specular color
    } else {
      this->sphere->setAmbient(1.0, 0.0, 0.2); // material ambient color
      this->sphere->setDiffuse(1.0, 1.0, 0.5); // material diffuse color
      this->sphere->setSpecular(0.8, 0.8, 0.8); // material specular color
    }
  }

  void setRadius(GLfloat r) {
    this->radius = r;
  }

  float getRadius() {
    return radius;
  }

  void rotateVector(Vector3f v, GLfloat angle, int degrees) {
    this->sphere->rotateVector(v, angle, degrees);
  }

  void rotateX(GLfloat angle, int degrees) {
    this->sphere->rotateX(angle, degrees);
  }

  void rotateY(GLfloat angle, int degrees) {
    this->sphere->rotateY(angle, degrees);
  }

  void rotateZ(GLfloat angle, int degrees) {
    this->sphere->rotateZ(angle, degrees);
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
