#version 120

attribute vec4 vertex_position;		// the vertex position (in the local space) from VBO
varying vec3 rVector;
uniform mat4 modelViewProjMat;

void main() {
	rVector = normalize(vertex_position.xyz);
  gl_Position = modelViewProjMat * vertex_position;
}
