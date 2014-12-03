#version 120

attribute vec4 vertex_position;		// the vertex position (in the local space) from VBO
attribute vec4 vertex_normal;		// the vertex normal (in the local space) from VBO

varying vec3 fragmentPosition;		// The fragment position output into the fragment shader
varying vec3 fragmentNormal;		// The fragment normal output into the fragment shader
varying vec3 reflectionVector;

uniform mat4 modelViewProjMat;
uniform mat4 modelViewMat;
uniform mat4 modelMat;
uniform vec4 camPos;

void main() {
	gl_Position = modelViewProjMat * vertex_position;
}
