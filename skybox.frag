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

varying vec3 rVector;
uniform samplerCube texCube;

void main() {
	gl_FragColor = vec4(1.0, 1.0, 0, 0);
	gl_FragColor = textureCube(texCube, rVector);
}
