/**
 * Camera class originally developed by Doron Nussbaum.
 *
 * Has been heavily modified by Ryan Seys - 100817604.
 */

#include "stdlib.h"
#include "stdio.h"

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ryan_camera.h"

/**
 * Camera constructor
 */
Camera::Camera(Vector3f posVec, Vector3f lookAtPoint, Vector3f upVec) {
  this->position = posVec;
  this->lookAtVector = lookAtPoint - position;
  this->upVector = upVec;

  modelMat = Matrix4f::identity();
  // setting up the viewpoint transformation
  viewMat = Matrix4f::cameraMatrix(this->position, this->lookAtVector, this->upVector);
  // setting up the projection transformation
  projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(60, 800.0/600.0, 1.0, 1000);

  this->refresh();
}

/**
 * When you reshape the window, you must update the projection matrix.
 *
 * @param w new window width
 * @param h new window height
 */
void Camera::reshape(GLfloat w, GLfloat h) {
  projMat = Matrix4f::symmetricPerspectiveProjectionMatrix(60, w/h, 1.0, 1000);
}

/**
 * camera destructor
 */
Camera::~Camera(void) {
  // destruct the camera
}

/**
 * Change the orientation of the camera (roll transformation)
 *
 * @param  angleDeg degrees to transform
 * @return          1 - if failed 0 - if successful
 */
int Camera::roll(float angleDeg) {
  // DO NOT DELETE -> use to create updateOrientation method + hints from PDF
  // Vector3f rotVector(0.0,0.0,0.0);
  // // Example code
  // // get rotation axis
  // rotVector = lookAtVector;
  // updateOrientation(rotVector, angleDeg);

  GLfloat angle = DegreeToRadians(angleDeg);
  // calculate the angle between upVector and rightVector for roll amount
  this->upVector = (upVector * cos(angle) + rightVector * sin(angle)).normalize();
  // update rightVector for new upVector
  this->rightVector = Vector3f::cross(upVector, lookAtVector);

  this->refresh();

  return 0;
}

/**
 * Change the orientation of the camera (pitch transformation)
 *
 * Rotate lookAtVector around the upVector
 *
 * @param  angleDeg degrees to transform
 * @return          1 - if failed 0 - if successful
 */
int Camera::pitch(float angleDeg) {
  GLfloat angle = DegreeToRadians(angleDeg);
  // calculate the angle between lookAtVector and upVector
  lookAtVector = (lookAtVector * cos(angle) + upVector * sin(angle)).normalize();
  // update upVector for the new lookAtVector
  upVector = Vector3f::cross(rightVector, lookAtVector);

  this->refresh();

  return 0;
}

/**
 * Change the orientation of the camera (yaw transformation)
 *
 * @param  angleDeg degrees to transform
 * @return          1 - if failed 0 - if successful
 */
int Camera::yaw(float angleDeg) {
  GLfloat angle = DegreeToRadians(angleDeg);
  // calculate angle between lookAtVector and rightVector to get yaw angle
  lookAtVector = (lookAtVector * cos(angle) + rightVector * sin(angle)).normalize();
  // update right angle for new lookAtVector
  rightVector = Vector3f::cross(lookAtVector, upVector);

  this->refresh();

  return 0;
}

/**
 * Obtains the current position of the camera.
 *
 * @return  Camera current position.
 */
Vector3f Camera::getPosition(void) {
  return position;
}

/**
 * Obtains the lookAt point of the camera.
 *
 * @return camera lookAt point
 */
Vector3f Camera::getLookAtPoint(void) {
  return (position + lookAtVector);
}

/**
 * Obtains the camera's up vector.
 *
 * @return  Camera upVector.
 */
Vector3f Camera::getUpVector(void) {
  return upVector;
}

/**
 * Changes the camera's position relative to its current position.
 * @param  dx [description]
 * @param  dy [description]
 * @param  dz [description]
 * @return    0 - success
 */
int Camera::changePositionDelta(float dx, float dy, float dz) {
  position.x += dx;
  position.y += dy;
  position.z += dz;

  return 0;
}

/**
 * Changes the camera's position relative to its current position.
 * @param  dv [description]
 * @return    0 - success
 */
int Camera::changePositionDelta(Vector3f *dv) {
  // TODO
  return 0;
}

/**
 * Changes the camera's position to a new position.
 *
 * @param  x [description]
 * @param  y [description]
 * @param  z [description]
 * @return   0 - success.
 */
int Camera::changeAbsPosition(float x, float y, float z) {
  // TODO
  return 0;
}

/**
 * Changes the camera's position to a new position.
 * @param  v [description]
 * @return   0 - success
 */
int Camera::changeAbsPosition(Vector3f *v) {
  // TODO
  return 0;
}

/**
 * Changes the camera's position along the LootAt vector
 * Move the camera forward by the numUnits along the lookAtVector.
 *
 * @param  numUnits [description]
 * @return          The new position.
 */
Vector3f Camera::moveForward(float numUnits) {
  this->position = position + (lookAtVector * numUnits);

  this->refresh();

  return position;
}

/**
 * Update the camera orientation.
 *
 * @param  rotVector [description]
 * @param  angleRad  [description]
 * @return           [description]
 */
int Camera::updateOrientation(Vector3f rotVector, float angleRad) {

  // TODO: Add Code

  // Vector3f xaxis(0.0,0.0,0.0);

  // upVector = CrossProduct(rightVector, lookAtVector);

  // create rotation matrix around a vector
  // rotate the camera (up vector and/or looAtVector)
  // update the  look-at and the up vectors using the x-axis vector
  // normalize the up  and look-at vectors

  return 0;
}

/**
 * Obtains the view transformation matrix.
 *
 * @return The transformation matrix
 */
Matrix4f Camera::getViewMatrix() {
  return projMat * viewMat * modelMat;
}

/**
 * Set the camera parameters.
 *
 * @param position    position vector of camera
 * @param lookAtPoint look at point of camera
 * @param upVector    up vector of camera
 */
void Camera::setCamera(Vector3f position, Vector3f lookAtPoint, Vector3f upVector) {
  this->position = position;
  this->lookAtVector = lookAtPoint - position;
  this->upVector = upVector;
  this->rightVector = Vector3f::cross(lookAtVector, upVector);
  this->upVector.normalize();
  this->lookAtVector.normalize();
  this->rightVector.normalize();
  viewMat = Matrix4f::cameraMatrix(this->position, this->getLookAtPoint(), this->upVector);
}

void Camera::refresh(void) {
  this->setCamera(this->position, this->getLookAtPoint(), this->upVector);
}

/**
 * Change the speed of the camera motion.
 *
 * @param  speed Speed of the camera
 * @return       0 - success
 */
int Camera::updateSpeed(float speed) {
  this->speed += speed;
  return 0;
}

float Camera::getSpeed(void) {
  return(speed);
}
