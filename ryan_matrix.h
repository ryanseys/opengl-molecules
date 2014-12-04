/**
 * This program is distributed for education purposes only and WITHOUT ANY WARRANTY.
 * This file is governed by the GNU General Public License.
 * Copyright 2014 Doron Nussbaum
 *
 * Cleaned, edited and fixed in some places by Ryan Seys - 100817604
 */

#ifndef RYAN_MATRIX
#define RYAN_MATRIX

#include <stdio.h>
#define _USE_MATH_DEFINES // for C++. The define is used to load the constants
#include <math.h>
#include "assert.h"
#include "ryan_vector.h"
#include <iostream>

#define DegreeToRadians(x) (float)(((x) * M_PI / 180.0f))
#define RadianToDegrees(x) (float)(((x) * 180.0f / M_PI))

class Matrix4f {

public:
  float *m;
  Vector4f vm[4];

  Matrix4f() {
    reset(0.0);
  }

  Matrix4f(Vector4f v0, Vector4f v1, Vector4f v2, Vector4f v3) {
    vm[0] = v0;
    vm[1] = v1;
    vm[2] = v2;
    vm[3] = v3;
    m = (float *) vm;
  }

  ~Matrix4f() {
    // std::cout << "Deleted Matrix4f" << std::endl;
  }

  // Static functions

  /**
   * Returns an identity matrix
   * @return Identity matrix
   */
  static Matrix4f identity() {
    Matrix4f m1;
    m1.reset(0);
    m1.vm[0].x = m1.vm[1].y = m1.vm[2].z = m1.vm[3].w = 1;
    return(m1);
  }

  /**
   * Creates a transpose of the matrix
   * Note: it does not change the internal values of the matrix
   * @return    A transposed matrix
   */
  static Matrix4f transpose(Matrix4f m1) {
    Matrix4f m2;

    m2.vm[0] = Vector4f(m1.vm[0].x, m1.vm[1].x, m1.vm[2].x, m1.vm[3].x);
    m2.vm[1] = Vector4f(m1.vm[0].y, m1.vm[1].y, m1.vm[2].y, m1.vm[3].y);
    m2.vm[2] = Vector4f(m1.vm[0].z, m1.vm[1].z, m1.vm[2].z, m1.vm[3].z);
    m2.vm[3] = Vector4f(m1.vm[0].w, m1.vm[1].w, m1.vm[2].w, m1.vm[3].w);

    return(m2);
  }

  static float determinant(Matrix4f m1) {
    float det = (
       m1.vm[0].x * m1.vm[1].y * m1.vm[2].z * m1.vm[3].w) +
      (m1.vm[0].x * m1.vm[1].z * m1.vm[2].w * m1.vm[3].y) +
      (m1.vm[0].x * m1.vm[1].w * m1.vm[2].y * m1.vm[3].z) +
      (m1.vm[0].y * m1.vm[1].x * m1.vm[2].w * m1.vm[3].z) +
      (m1.vm[0].y * m1.vm[1].z * m1.vm[2].x * m1.vm[3].w) +
      (m1.vm[0].y * m1.vm[1].w * m1.vm[2].z * m1.vm[3].x) +
      (m1.vm[0].z * m1.vm[1].x * m1.vm[2].y * m1.vm[3].w) +
      (m1.vm[0].z * m1.vm[1].y * m1.vm[2].w * m1.vm[3].x) +
      (m1.vm[0].z * m1.vm[1].w * m1.vm[2].x * m1.vm[3].y) +
      (m1.vm[0].w * m1.vm[1].x * m1.vm[2].z * m1.vm[3].y) +
      (m1.vm[0].w * m1.vm[1].y * m1.vm[2].x * m1.vm[3].z) +
      (m1.vm[0].w * m1.vm[1].z * m1.vm[2].y * m1.vm[3].x) -
      (m1.vm[0].x * m1.vm[1].y * m1.vm[2].w * m1.vm[3].z) -
      (m1.vm[0].x * m1.vm[1].z * m1.vm[2].y * m1.vm[3].w) -
      (m1.vm[0].x * m1.vm[1].w * m1.vm[2].z * m1.vm[3].y) -
      (m1.vm[0].y * m1.vm[1].x * m1.vm[2].z * m1.vm[3].w) -
      (m1.vm[0].y * m1.vm[1].z * m1.vm[2].w * m1.vm[3].x) -
      (m1.vm[0].y * m1.vm[1].w * m1.vm[2].x * m1.vm[3].z) -
      (m1.vm[0].z * m1.vm[1].x * m1.vm[2].w * m1.vm[3].y) -
      (m1.vm[0].z * m1.vm[1].y * m1.vm[2].x * m1.vm[3].w) -
      (m1.vm[0].z * m1.vm[1].w * m1.vm[2].y * m1.vm[3].x) -
      (m1.vm[0].w * m1.vm[1].x * m1.vm[2].y * m1.vm[3].z) -
      (m1.vm[0].w * m1.vm[1].y * m1.vm[2].z * m1.vm[3].x) -
      (m1.vm[0].w * m1.vm[1].z * m1.vm[2].x * m1.vm[3].y);
    return det;
  }

  static Matrix4f inverse(Matrix4f m1) {
    Matrix4f inv;
    float det = determinant(m1);
    if(det == 0) {
      printf("Determinant was zero.\n");
      return m1;
    } else {
      // printf("Before inverse:\n");
      // printf("%f %f %f %f\n", m1.vm[0].x, m1.vm[0].y, m1.vm[0].z, m1.vm[0].w);
      // printf("%f %f %f %f\n", m1.vm[1].x, m1.vm[1].y, m1.vm[1].z, m1.vm[1].w);
      // printf("%f %f %f %f\n", m1.vm[2].x, m1.vm[2].y, m1.vm[2].z, m1.vm[2].w);
      // printf("%f %f %f %f\n", m1.vm[3].x, m1.vm[3].y, m1.vm[3].z, m1.vm[3].w);

      float b11 = ((m1.vm[1].y * m1.vm[2].z * m1.vm[3].w) +
      (m1.vm[1].z * m1.vm[2].w * m1.vm[3].y) +
      (m1.vm[1].w * m1.vm[2].y * m1.vm[3].z) -
      (m1.vm[1].y * m1.vm[2].w * m1.vm[3].z) -
      (m1.vm[1].z * m1.vm[2].y * m1.vm[3].w) -
      (m1.vm[1].w * m1.vm[2].z * m1.vm[3].y));
      float b12 = ((m1.vm[0].y * m1.vm[2].w * m1.vm[3].z) +
      (m1.vm[0].z * m1.vm[2].y * m1.vm[3].w) +
      (m1.vm[0].w * m1.vm[2].z * m1.vm[3].y) -
      (m1.vm[0].y * m1.vm[2].z * m1.vm[3].w) -
      (m1.vm[0].z * m1.vm[2].w * m1.vm[3].y) -
      (m1.vm[0].w * m1.vm[2].y * m1.vm[3].z));
      float b13 = ((m1.vm[0].y * m1.vm[1].z * m1.vm[3].w) +
      (m1.vm[0].z * m1.vm[1].w * m1.vm[3].y) +
      (m1.vm[0].w * m1.vm[1].y * m1.vm[3].z) -
      (m1.vm[0].y * m1.vm[1].w * m1.vm[3].z) -
      (m1.vm[0].z * m1.vm[1].y * m1.vm[3].w) -
      (m1.vm[0].w * m1.vm[1].z * m1.vm[3].y));
      float b14 = ((m1.vm[0].y * m1.vm[1].w * m1.vm[2].z) +
      (m1.vm[0].z * m1.vm[1].y * m1.vm[2].w) +
      (m1.vm[0].w * m1.vm[1].z * m1.vm[2].y) -
      (m1.vm[0].y * m1.vm[1].z * m1.vm[2].w) -
      (m1.vm[0].z * m1.vm[1].w * m1.vm[2].y) -
      (m1.vm[0].w * m1.vm[1].y * m1.vm[2].z));
      float b21 = ((m1.vm[1].x * m1.vm[2].w * m1.vm[3].z) +
      (m1.vm[1].z * m1.vm[2].x * m1.vm[3].w) +
      (m1.vm[1].w * m1.vm[2].z * m1.vm[3].x) -
      (m1.vm[1].x * m1.vm[2].z * m1.vm[3].w) -
      (m1.vm[1].z * m1.vm[2].w * m1.vm[3].x) -
      (m1.vm[1].w * m1.vm[2].x * m1.vm[3].z));
      float b22 = ((m1.vm[0].x * m1.vm[2].z * m1.vm[3].w) +
      (m1.vm[0].z * m1.vm[2].w * m1.vm[3].x) +
      (m1.vm[0].w * m1.vm[2].x * m1.vm[3].z) -
      (m1.vm[0].x * m1.vm[2].w * m1.vm[3].z) -
      (m1.vm[0].z * m1.vm[2].x * m1.vm[3].w) -
      (m1.vm[0].w * m1.vm[2].z * m1.vm[3].x));
      float b23 = ((m1.vm[0].x * m1.vm[1].w * m1.vm[3].z) +
      (m1.vm[0].z * m1.vm[1].x * m1.vm[3].w) +
      (m1.vm[0].w * m1.vm[1].z * m1.vm[3].x) -
      (m1.vm[0].x * m1.vm[1].z * m1.vm[3].w) -
      (m1.vm[0].z * m1.vm[1].w * m1.vm[3].x) -
      (m1.vm[0].w * m1.vm[1].x * m1.vm[3].z));
      float b24 = ((m1.vm[0].x * m1.vm[1].z * m1.vm[2].w) +
      (m1.vm[0].z * m1.vm[1].w * m1.vm[2].x) +
      (m1.vm[0].w * m1.vm[1].x * m1.vm[2].z) -
      (m1.vm[0].x * m1.vm[1].w * m1.vm[2].z) -
      (m1.vm[0].z * m1.vm[1].x * m1.vm[2].w) -
      (m1.vm[0].w * m1.vm[1].z * m1.vm[2].x));
      float b31 = ((m1.vm[1].x * m1.vm[2].y * m1.vm[3].w) +
      (m1.vm[1].y * m1.vm[2].w * m1.vm[3].x) +
      (m1.vm[1].w * m1.vm[2].x * m1.vm[3].y) -
      (m1.vm[1].x * m1.vm[2].w * m1.vm[3].y) -
      (m1.vm[1].y * m1.vm[2].x * m1.vm[3].w) -
      (m1.vm[1].w * m1.vm[2].y * m1.vm[3].x));
      float b32 = ((m1.vm[0].x * m1.vm[2].w * m1.vm[3].y) +
      (m1.vm[0].y * m1.vm[2].x * m1.vm[3].w) +
      (m1.vm[0].w * m1.vm[2].y * m1.vm[3].x) -
      (m1.vm[0].x * m1.vm[2].y * m1.vm[3].w) -
      (m1.vm[0].y * m1.vm[2].w * m1.vm[3].x) -
      (m1.vm[0].w * m1.vm[2].x * m1.vm[3].y));
      float b33 = ((m1.vm[0].x * m1.vm[1].y * m1.vm[3].w) +
      (m1.vm[0].y * m1.vm[1].w * m1.vm[3].x) +
      (m1.vm[0].w * m1.vm[1].x * m1.vm[3].y) -
      (m1.vm[0].x * m1.vm[1].w * m1.vm[3].y) -
      (m1.vm[0].y * m1.vm[1].x * m1.vm[3].w) -
      (m1.vm[0].w * m1.vm[1].y * m1.vm[3].x));
      float b34 = ((m1.vm[0].x * m1.vm[1].w * m1.vm[2].y) +
      (m1.vm[0].y * m1.vm[1].x * m1.vm[2].w) +
      (m1.vm[0].w * m1.vm[1].y * m1.vm[2].x) -
      (m1.vm[0].x * m1.vm[1].y * m1.vm[2].w) -
      (m1.vm[0].y * m1.vm[1].w * m1.vm[2].x) -
      (m1.vm[0].w * m1.vm[1].x * m1.vm[2].y));
      float b41 = ((m1.vm[1].x * m1.vm[2].z * m1.vm[3].y) +
      (m1.vm[1].y * m1.vm[2].x * m1.vm[3].z) +
      (m1.vm[1].z * m1.vm[2].y * m1.vm[3].x) -
      (m1.vm[1].x * m1.vm[2].y * m1.vm[3].z) -
      (m1.vm[1].y * m1.vm[2].z * m1.vm[3].x) -
      (m1.vm[1].z * m1.vm[2].x * m1.vm[3].y));
      float b42 = ((m1.vm[0].x * m1.vm[2].y * m1.vm[3].z) +
      (m1.vm[0].y * m1.vm[2].z * m1.vm[3].x) +
      (m1.vm[0].z * m1.vm[2].x * m1.vm[3].y) -
      (m1.vm[0].x * m1.vm[2].z * m1.vm[3].y) -
      (m1.vm[0].y * m1.vm[2].x * m1.vm[3].z) -
      (m1.vm[0].z * m1.vm[2].y * m1.vm[3].x));
      float b43 = ((m1.vm[0].x * m1.vm[1].z * m1.vm[3].y) +
      (m1.vm[0].y * m1.vm[1].x * m1.vm[3].z) +
      (m1.vm[0].z * m1.vm[1].y * m1.vm[3].x) -
      (m1.vm[0].x * m1.vm[1].y * m1.vm[3].z) -
      (m1.vm[0].y * m1.vm[1].z * m1.vm[3].x) -
      (m1.vm[0].z * m1.vm[1].x * m1.vm[3].y));
      float b44 = ((m1.vm[0].x * m1.vm[1].y * m1.vm[2].z) +
      (m1.vm[0].y * m1.vm[1].z * m1.vm[2].x) +
      (m1.vm[0].z * m1.vm[1].x * m1.vm[2].y) -
      (m1.vm[0].x * m1.vm[1].z * m1.vm[2].y) -
      (m1.vm[0].y * m1.vm[1].x * m1.vm[2].z) -
      (m1.vm[0].z * m1.vm[1].y * m1.vm[2].x));

      inv.vm[0].x = b11;
      inv.vm[0].y = b12;
      inv.vm[0].z = b13;
      inv.vm[0].w = b14;

      inv.vm[1].x = b21;
      inv.vm[1].y = b22;
      inv.vm[1].z = b23;
      inv.vm[1].w = b24;

      inv.vm[2].x = b31;
      inv.vm[2].y = b32;
      inv.vm[2].z = b33;
      inv.vm[2].w = b34;

      inv.vm[3].x = b41;
      inv.vm[3].y = b42;
      inv.vm[3].z = b43;
      inv.vm[3].w = b44;

      inv = inv * (1.0 / det);

      // printf("Result:\n");
      // printf("%f %f %f %f\n", inv.vm[0].x, inv.vm[0].y, inv.vm[0].z, inv.vm[0].w);
      // printf("%f %f %f %f\n", inv.vm[1].x, inv.vm[1].y, inv.vm[1].z, inv.vm[1].w);
      // printf("%f %f %f %f\n", inv.vm[2].x, inv.vm[2].y, inv.vm[2].z, inv.vm[2].w);
      // printf("%f %f %f %f\n", inv.vm[3].x, inv.vm[3].y, inv.vm[3].z, inv.vm[3].w);

      return inv;
    }
  }

  /**
   * Return a scale matrix.
   * Note: Internal parameters are not changed.
   * Note: It resets the data first.
   *
   * @param  scaleX [description]
   * @param  scaleY [description]
   * @param  scaleZ [description]
   * @return        A scale matrix.
   */
  static Matrix4f scale(float scaleX, float scaleY, float scaleZ) {
    Matrix4f m1;
    m1 = identity();
    m1.vm[0].x *= scaleX;
    m1.vm[1].y *= scaleY;
    m1.vm[2].z *= scaleZ;
    return m1;
  }

  /**
   * Set the matrix as a pitch rotation matrix
   * if degree is true (!=0) then angle is given in degrees
   * otherwise angle is given in radians.
   *
   * @param  angle  angle provided
   * @param  degree if true, angle provided is degrees
   * @return        The rotation matrix.
   */
  static Matrix4f rotateX(float angle, int degree) {
    Matrix4f m1;
    if (degree) angle = DegreeToRadians(angle);
    m1 = identity();
    m1.vm[1].y = m1.vm[2].z = cos(angle);
    m1.vm[1].z = -sin(angle);
    m1.vm[2].y = -m1.vm[1].z;
    return(m1);
  }

  /**
   * Set the matrix as a yaw rotation matrix
   * if degree is true (!=0) then angle is given in degrees
   * otherwise angle is given in radians.
   *
   * @param  angle  angle provided
   * @param  degree if true, angle provided is degrees
   * @return        The rotation matrix.
   */
  static Matrix4f rotateY(float angle, int degree) {
    Matrix4f m1;
    if (degree) angle = DegreeToRadians(angle);
    m1 = identity();
    m1.vm[0].x = m1.vm[2].z = cos(angle);
    m1.vm[0].z = sin(angle);
    m1.vm[2].x = -m1.vm[0].z;
    return(m1);
  }

  /**
   * Set the matrix as a roll rotation matrix
   * if degree is true (!=0) then angle is given in degrees
   * otherwise angle is given in radians.
   *
   * @param  angle  angle provided
   * @param  degree if true, angle provided is degrees
   * @return        The rotation matrix.
   */
  static Matrix4f rotateZ(float angle, int degree) {
    Matrix4f m1;
    if (degree) angle = DegreeToRadians(angle);
    m1 = identity();
    m1.vm[0].x = m1.vm[1].y = cos(angle);
    m1.vm[0].y = -sin(angle);
    m1.vm[1].x = -m1.vm[0].y;

    return(m1);
  }

  // set the matrix as a roll pitch and yaw rotation matrix
  // the resulting matrix M=Mat(yaw)*mat(pitch)*mat(roll)
  // if degree is true (!=0) then all angles are given in degrees
  // otherwise angle is given in radians
  static Matrix4f rotateRollPitchYaw(float angleRoll, float anglePitch, float angleYaw, int degree) {
    Matrix4f m1;
    m1 = identity();
    m1 = rotateY(angleYaw, degree) * rotateX(anglePitch, degree) * rotateZ(angleRoll, degree);

    return(m1);
  }

  /**
   * Set the matrix as a rotation around a vector where only a rotation vector
   * is given if degree is true (!=0) then all angles are given in degrees
   * otherwise angle is given in radians.
   *
   * @param  Vector3f [description]
   * @param  angle    [description]
   * @param  degree   [description]
   * @return          [description]
   */
  static Matrix4f rotateVector( Vector3f u, float angle, bool isDegree = true) {
    u.normalize();
    if (isDegree) angle = DegreeToRadians(angle);
    float x = u.x;
    float y = u.y;
    float z = u.z;
    float c = cosf( angle );
    float s = sinf( angle );

    Matrix4f m1;
    m1 = identity();
    m1.vm[0] = Vector4f( c + x*x*(1-c),     x*y*(1-c) - z*s,    x*z*(1-c) + y*s,    0);
    m1.vm[1] = Vector4f( x*y*(1-c) + z*s,   c + y*y*(1-c),      y*z*(1-c) - x*s,    0);
    m1.vm[2] = Vector4f( z*x*(1-c) - y*s,   z*y*(1-c) + x*s,    c + z*z*(1-c),      0);
    m1.vm[3] = Vector4f( 0,                 0,                  0,                  1);
    return(m1);
  }

  // static Matrix4f rotateAroundTwoPoints(Vector3f p1, Vector3f p2) {
  //   Matrix4f m1;
  //   m1 = identity();

  //   m1 = translation(-p1.x, -p1.y, -p1.z);

  //   return(m1);
  // }

  /**
   * set the matrix as a rotation around a vector where only a rotation vector
   * is given if degree is true (!=0) then all angles are given in degrees
   * otherwise angle is given in radians.
   *
   * @param  u      [description]
   * @param  angle  [description]
   * @param  degree [description]
   * @return        [description]
   */
  static Matrix4f rotateVector(Vector4f u, float angle, int degree) {
    Matrix4f m1;
    Vector3f v = Vector3f(u.x, u.y, u.z);
    m1 = rotateVector(v, angle, degree);

    return(m1);
  }

  static Matrix4f translation(float dx, float dy, float dz) {
    Matrix4f m1;

    m1 = identity();
    m1.vm[0].w = dx;
    m1.vm[1].w = dy;
    m1.vm[2].w = dz;
    return(m1);
  }

  static Matrix4f cameraMatrix(Vector3f position, Vector3f lookAtPoint, Vector3f upVector) {
    Matrix4f m1;
    Vector3f u,v,n;

    m1 = identity();
    n = position-lookAtPoint;
    n.normalize();
    upVector.normalize();

    u = Vector3f::cross(upVector, n);
    u.normalize();
    v = Vector3f::cross(n, u);

    m1.vm[0] = Vector4f(u,0);
    m1.vm[1] = Vector4f(v,0);
    m1.vm[2] = Vector4f(n,0);
    m1.vm[3] = Vector4f(0,0,0,1);
    m1.vm[0].w = Vector3f::dot(-u,position);
    m1.vm[1].w = Vector3f::dot(-v,position);
    m1.vm[2].w = Vector3f::dot(-n,position);

    return(m1);
  }

  static Matrix4f symmetricPerspectiveProjectionMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane) {
    Matrix4f m1;
    m1 = identity();

    float cot = 1/tan(DegreeToRadians(fieldOfView / 2.0));
    m1.vm[0].x = cot/aspectRatio;
    m1.vm[1].y = cot;
    m1.vm[2].z = (nearPlane+farPlane)/(nearPlane-farPlane);
    m1.vm[2].w = 2.0*nearPlane*farPlane/(nearPlane-farPlane);
    m1.vm[3].z = -1;
    m1.vm[3].w = 0;

    return(m1);
  }

  static Matrix4f frustumProjectionMatrix(float winMinX, float winMinY, float winMaxX,float winMaxY,float nearPlane, float farPlane) {
    Matrix4f m1;
    m1 = identity();

    m1.vm[0].x = 2*nearPlane/(winMaxX - winMinX);   // check why it is not "-2"
    m1.vm[0].z = (winMaxX + winMinX) /  (winMaxX - winMinX);
    m1.vm[1].y = 2*nearPlane/(winMaxY - winMinY);   // check why it is not "-2"
    m1.vm[1].z = (winMaxY + winMinY) /  (winMaxY - winMinY);
    m1.vm[2].z = (nearPlane + farPlane) / (nearPlane - farPlane);
    m1.vm[2].w = (2*nearPlane * farPlane) / (nearPlane - farPlane);
    m1.vm[3].z = -1;
    m1.vm[3].w = 0;

    return(m1);
  }

  /**
   * Resets all the values of the matrix to "value".
   * Sets all 16 values of the matrix to value.
   * @param value Value to set all the numbers to.
   */
  inline void reset(float value){
    int i;
    for (i=0; i <=3; i++) {
      vm[i]= Vector4f(value, value, value, value);
    }
  }

  // operators
  inline Matrix4f operator*(const Matrix4f& rhs) const {
    Matrix4f m1, m2;
    int i, j;

    m2 = transpose(rhs);
    m1.m = (float *) m1.vm;
    for (i = 0 ; i < 4 ; i++) {
      for (j = 0 ; j < 4 ; j++) {
        m1.m[i*4+j] = Vector4f::dot(vm[i], m2.vm[j]);
      }
    }

    return(m1);
  }

  inline Matrix4f operator*(const float f) const {
    Matrix4f m1;
    int i;
    for (i = 0; i < 4; i++) {
      m1.vm[i] = vm[i]*f;
    }

    return(m1);
  }

  friend Matrix4f operator*(const float f, Matrix4f m1) {
    return(m1*f);
  }

  inline Matrix4f operator+(const Matrix4f& rhs) const {
    Matrix4f m1, m2;
    int i;

    for (i = 0 ; i < 4 ; i++) {
      m1.vm[i] = vm[i]+rhs.vm[i];
    }

    return(m1);
  }

  inline Matrix4f operator-(const Matrix4f& rhs) const {
    Matrix4f m1, m2;
    int i;

    for (i = 0 ; i < 4 ; i++) {
      m1.vm[i] = vm[i]-rhs.vm[i];
    }

    return(m1);
  }

  // multiply the matrix by a vector
  Vector4f operator*(const Vector4f& v) const {
    Vector4f r;
    r.x = Vector4f::dot(vm[0],v);
    r.y = Vector4f::dot(vm[1],v);
    r.z = Vector4f::dot(vm[2],v);
    r.w = Vector4f::dot(vm[3],v);
    return r;
  }
};

#endif
