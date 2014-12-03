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

varying vec3 fragmentPosition;		// The fragment position interpolated from the vertex shader
varying vec3 reflectionVector;
varying float time;

uniform samplerCube texCube;

void main() {
	gl_FragColor = vec4(0, 0, 0, 1);
}
