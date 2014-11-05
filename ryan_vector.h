/**
 * This program is distributed for education purposes only and WITHOUT ANY WARRANTY.
 * This file is governed by the GNU General Public License.
 * Copyright 2014 Doron Nussbaum
 *
 * Cleaned and edited by Ryan Seys - 100817604
 */
#ifndef RYAN_VECTOR
#define RYAN_VECTOR

#include <stdio.h>
#define _USE_MATH_DEFINES // for C++. The define is used to load the constants
#include <math.h>
#include "assert.h"

#define DegreeToRadians(x) (float)(((x) * M_PI / 180.0f))
#define RadianToDegrees(x) (float)(((x) * 180.0f / M_PI))

struct Vector2f {
  float x;
  float y;

  Vector2f() {
    x=0;
    y=0;
  }

  Vector2f(float x, float y) {
    this->x = x;
    this->y = y;
  }
};

struct Vector3f {
  float x;
  float y;
  float z;

  // constructors
  Vector3f() {
    x = 0;
    y = 0;
    z = 0;
  }

  Vector3f(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  Vector3f(Vector2f u, float z) {
    this->x = u.x;
    this->y = u.y;
    this->z = z;
  }

  // unary operators
  Vector3f operator-() {
    return(Vector3f(-x,-y,-z));
  }

  // binary operators
  Vector3f operator+(const Vector3f& u) const {
    return(Vector3f(x+u.x,y +u.y, z+u.z));
  }

  Vector3f operator-(const Vector3f& u) const {
    return(Vector3f(x-u.x, y-u.y, z-u.z));
  }

  Vector3f operator*(const Vector3f& u) const {
    return(Vector3f(x*u.x, y*u.y, z*u.z));
  }

  Vector3f operator*(const float f) const {
    return(Vector3f(x*f, y*f, z*f));
  }

  Vector3f operator/(const float f) const {
    assert(f!=0);
    return(Vector3f(x/f, y/f, z/f));
  }

  friend Vector3f operator*(const float f, const Vector3f& u) {
    return(u*f);
  }

  Vector3f& operator+=(const Vector3f& r) {
    x += r.x;
    y += r.y;
    z += r.z;

    return *this;
  }

  Vector3f& operator-=(const Vector3f& r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;

    return *this;
  }

  Vector3f& operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;

    return *this;
  }

  Vector3f& operator/=(float f) {
    assert(f!=0);
    x /= f;
    y /= f;
    z /= f;

    return *this;
  }

  // returns the magnitude of the vector
  float length() {
    return(sqrt(this->x*this->x+this->y*this->y+this->z*this->z));
  }

  // normalizes the vector
  Vector3f& normalize() {
    float length = this->length();
    *this /= length;
    return *this;
  }

  void print(void) {
    printf("(%f, %f, %f)\n", this->x, this->y, this->z);
  }

  static inline float dot(const Vector3f& u, const Vector3f& v) {
    return(u.x * v.x + u.y*v.y + u.z*v.z);
  }

  static inline float length(const Vector3f& u) {
    return(sqrt(dot(u,u)));
  }

  static Vector3f normalize(const Vector3f& u) {
    float len = length(u);
    return(u/len);
  }

  static Vector3f cross(const Vector3f& u,  const Vector3f& v) {
    Vector3f vv;
    vv.x =u.y*v.z-v.y*u.z;
    vv.y=-1*(u.x*v.z-v.x*u.z);
    vv.z =u.x*v.y-v.x*u.y;
    return(Vector3f(u.y*v.z-v.y*u.z, -1*(u.x*v.z-v.x*u.z), u.x*v.y-v.x*u.y));
  }

  static float cross_value_value(const Vector3f& u,  const Vector3f& v) {
    return(u.y*v.z-v.y*u.z + -1*(u.x*v.z-v.x*u.z) + u.x*v.y-v.x*u.y);
  }
};

struct Vector4f {
  float x;
  float y;
  float z;
  float w;

  // constructors
  Vector4f() {
    x = 0;
    y = 0;
    z = 0;
    w = 0;
  }

  Vector4f(float f) {
    x = y = z = w = f;
  }

  Vector4f(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }

  Vector4f(Vector2f u, float z, float w) {
    this->x = u.x;
    this->y = u.y;
    this->z = z;
    this->w = w;
  }

  Vector4f(Vector3f u, float w) {
    this->x = u.x;
    this->y = u.y;
    this->z = u.z;
    this->w = w;
  }

  Vector4f setValue(float f) {
    x= y= z= w=f;
  }

  // unary operators
  Vector4f operator-() {
    return(Vector4f(-x,-y,-z,-w));
  }

  // binary operators
  Vector4f operator+(const Vector4f& u) const {
    return(Vector4f(x+u.x,y +u.y, z+u.z, w+u.w));
  }

  Vector4f operator-(const Vector4f& u) const {
    return(Vector4f(x-u.x, y-u.y, z-u.z, w-u.w));
  }

  Vector4f operator*(const Vector4f& u) const {
    return(Vector4f(x*u.x, y*u.y, z*u.z, w*u.w));
  }

  Vector4f operator*(const float f) const {
    return(Vector4f(x*f, y*f, z*f, w*f));
  }

  Vector4f operator/(const float f) const {
    assert(f!=0);
    return(Vector4f(x/f, y/f, z/f, w/f));
  }

  friend Vector4f operator*(const float f, const Vector4f& u) {
    return(u*f);
  }

  Vector4f& operator+=(const Vector4f& r) {
    x += r.x;
    y += r.y;
    z += r.z;
    w += r.w;
    return *this;
  }

  Vector4f& operator-=(const Vector4f& r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    w -= r.w;

    return *this;
  }

  Vector4f& operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
  }

  Vector4f& operator/=(float f) {
    assert(f!=0);
    x /= f;
    y /= f;
    z /= f;
    w /= f;

    return *this;
  }

  // returns the magnitude of the vector
  float length() {
    return(sqrt(this->x*this->x+this->y*this->y+this->z*this->z+this->w*this->w));
  }

  // normalizes the vector
  void normalize() {
    float length = this->length();
    *this /= length;
  }

  static inline float dot(const Vector4f& u, const Vector4f& v) {
    return(u.x * v.x + u.y*v.y + u.z*v.z + u.w*v.w);
  }

  static Vector3f cross(const Vector4f& u,  const Vector4f& v) {
    Vector3f vv;
    vv.x =u.y*v.z-v.y*u.z;
    vv.y=-1*(u.x*v.z-v.x*u.z);
    vv.z =u.x*v.y-v.x*u.y;
    return(Vector3f(u.y*v.z-v.y*u.z, -1*(u.x*v.z-v.x*u.z), u.x*v.y-v.x*u.y));
  }


  static float cross_value(const Vector4f& u,  const Vector4f& v) {
    return(u.y*v.z-v.y*u.z + -1*(u.x*v.z-v.x*u.z) + u.x*v.y-v.x*u.y);
  }
};

#endif
