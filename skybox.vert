#version 120

//
//Author: Doron Nussbaum (C) 2014 All Rights Reserved.
//-----------------------------------------------------
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

uniform vec4 cameraPosition;
attribute vec4 vertex_position;		// the vertex position (in the local space) from VBO
attribute vec4 vertex_normal;		// the vertex normal (in the local space) from VBO
attribute vec2 texCoord;			// coordiantes of the texture

varying vec3 rVector;
uniform float t;
varying float time;

uniform mat4 modelViewProjMat;
uniform mat4 modelViewMat;

void main() {
	rVector = normalize(vertex_position.xyz);
  gl_Position = modelViewProjMat * vertex_position;

	time = t;
}
