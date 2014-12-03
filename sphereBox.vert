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

attribute vec4 vertex_position;		// the vertex position (in the local space) from VBO
attribute vec4 vertex_normal;		// the vertex normal (in the local space) from VBO

varying vec3 fragmentPosition;		// The fragment position output into the fragment shader
varying vec3 fragmentNormal;		// The fragment normal output into the fragment shader
varying vec3 reflectionVector;
varying vec3 refractVector;

uniform mat4 modelViewProjMat;
uniform mat4 modelViewMat;
uniform mat4 modelMat;
uniform vec3 camPos;

void main() {
	float inRefractFactor = 1.0;  // refracting ray factor of the in material
	float outRefractFactor = 1.31; // refracting ray factor of the out material

	vec4 vtx;
	vec3 viewVec;
	float eta;

	// set the ratio between the ouside material and the inside material for refracting a vector
	eta = inRefractFactor/outRefractFactor;

	vtx = modelMat * vertex_position;
	fragmentPosition = vtx.xyz;

	// get the normal
	vtx   = modelMat * vertex_normal;
	fragmentNormal   = vtx.xyz;
	fragmentNormal = normalize(fragmentNormal);

	// position of fragment with projection matrix
	gl_Position = modelViewProjMat * vertex_position;

	// get the view vector (from the eye to the fragment
	viewVec = fragmentPosition - camPos;
	viewVec = normalize(viewVec);

	// compute the reflection and the refractding vectors
	reflectionVector = reflect(viewVec, fragmentNormal);
	refractVector = refract(viewVec,fragmentNormal,eta);
}
