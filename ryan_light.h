#ifndef RYAN_LIGHT
#define RYAN_LIGHT

#include "ryan_vector.h"
#include "ryan_matrix.h"

class Light {
public:
  Vector4f position;
  Vector4f ambient;
  Vector4f diffuse;
  Vector4f specular;
  Vector4f lookAtPoint;
  GLfloat angularAtten;
  GLfloat coneAngle;

  Light() {
    this->position = Vector4f(0, 0, 0, 0);
    this->ambient = Vector4f(0, 0, 0, 0);
    this->diffuse = Vector4f(0, 0, 0, 0);
    this->specular = Vector4f(0, 0, 0, 0);

    // Used only for spotlights
    this->lookAtPoint = Vector4f(0, 0, 0, 0);
    this->angularAtten = 23;
    this->coneAngle = 35;
  }

  void setAmbient(GLfloat r, GLfloat g, GLfloat b) {
    this->ambient = Vector4f(r, g, b, 0.0);
  }

  void setDiffuse(GLfloat r, GLfloat g, GLfloat b) {
    this->diffuse = Vector4f(r, g, b, 0.0);
  }

  void setSpecular(GLfloat r, GLfloat g, GLfloat b) {
    this->specular = Vector4f(r, g, b, 0.0);
  }

  void setPosition(GLfloat x, GLfloat y, GLfloat z) {
    this->position = Vector4f(x, y, z, 0);
  }

  // Below are properties only used for spotlights
  void setLookAtPoint(GLfloat x, GLfloat y, GLfloat z) {
    this->lookAtPoint = Vector4f(x, y, z, 0);
  }

  void setAngularAttenuation(GLfloat att) {
    this->angularAtten = att;
  }

  void setConeAngle(GLfloat angle) {
    this->coneAngle = angle;
  }
};

#endif
