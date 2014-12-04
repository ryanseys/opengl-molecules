/**
 * Phong lighting model - vertex shader.
 * Ryan Seys - 100817604
 */
#version 120

// world properties
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 normalMat;

attribute vec4 vPosition;   // the vertex position (in the local space) from VBO
attribute vec4 vNormal;   // the vertex normal (in the local space) from VBO

varying vec4 color;
varying vec4 N; // export to fragment shader
varying vec4 v; // export to fragment shader

uniform vec4 ambientMat;
uniform vec4 diffuseMat;
uniform vec4 specMat;
uniform float specPow;

varying vec3 reflectionVector;

void main(void) {
  mat4 modelViewMat = viewMat * modelMat;
  mat4 modelViewProjMat = projMat * modelViewMat;

  v = vec4(modelViewMat * vPosition);
  N = normalize(normalMat * vNormal);

  vec4 viewVec = vPosition - vec4(0.0, 1.0, 10.0, 1.0); // vPosition - camPos
  viewVec = normalize(viewVec);

  reflectionVector = reflect(viewVec.xyz, N.xyz);
  gl_Position = modelViewProjMat * vPosition;
}
