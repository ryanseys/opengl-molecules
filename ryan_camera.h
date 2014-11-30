#ifndef RYAN_CAMERA
#define RYAN_CAMERA

#pragma once
#include "ryan_matrix.h"

class Camera {

private:
  int updateOrientation(Vector3f rotVector, float angleRad); // update the camera's orientation in space

public:
  Matrix4f rotMat;
  Vector3f position;
  Vector3f upVector;
  Vector3f lookAtVector;
  Vector3f rightVector;
  Matrix4f modelMat;
  Matrix4f viewMat;
  Matrix4f projMat;
  Matrix4f getViewMatrix();
  Matrix4f getProjMatrix();
  float speed;

  // Camera(void);
  Camera(Vector3f posVec, Vector3f lookAtVec, Vector3f upVec);
  ~Camera(void);
  int roll(float angleDeg);
  int pitch(float angleDeg);
  int yaw(float angleDeg);
  Vector3f getPosition(void);
  Vector3f getLookAtPoint(void);
  Vector3f getUpVector(void);
  int changePositionDelta(float dx, float dy, float dz); // change position by relative amount
  int changePositionDelta(Vector3f *dv);        // change position by relative amount
  int changeAbsPosition(float x, float y, float z);    // change to a new position in space
  int changeAbsPosition(Vector3f *v);         // change to a new position in space
  Vector3f moveForward(float numUnits);  // moves the camera forward by the numUnits units along the lookAtVector
  Vector3f moveRight(float numUnits);
  // void setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector);
  int updateSpeed(float speed);
  float getSpeed(void);
  Camera(GLfloat w, GLfloat h, GLfloat d);
  void setCamera(Vector3f, Vector3f, Vector3f);
  void refresh(void);
  void reshape(GLfloat w, GLfloat h);
};

#endif
