#version 120

varying vec3 rVector;
uniform samplerCube texCube;

void main() {
	gl_FragColor = textureCube(texCube, rVector);
}
