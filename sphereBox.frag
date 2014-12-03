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
varying vec3 refractVector;

varying float time;

uniform samplerCube texCube;
uniform int refractFlag;

void main()
{



	if (refractFlag == 1) 	gl_FragColor = textureCube(texCube, refractVector);
	else gl_FragColor = textureCube(texCube, reflectionVector);
	// gl_FragColor = vec4(1.0,0,0,1);
 }
