/**
 * Phong lighting model - fragment shader.
 * Ryan Seys - 100817604
 */
#version 120
// Material color components
uniform vec4 materialAmb;
uniform vec4 materialDiff;
uniform vec4 materialSpec;

// Light components
uniform vec4 lightAmb;
uniform vec4 lightDiff;
uniform vec4 lightSpec;
uniform vec4 lightPos;

// Specular power (shininess factor)
uniform float shininess;

// Spotlight components
uniform vec4 spotPos;
uniform vec4 spotLookAtPnt;
uniform float spotAngAtten;
uniform float spotConeAngle;

uniform int refractFlag;
uniform samplerCube texCube;
varying vec3 reflectionVector;

varying vec4 v; // from vertex shader
varying vec4 N; // from vertex shader

void main (void) {
  vec4 L = normalize(lightPos - v);
  vec4 E = normalize(-v);
  vec4 R = normalize(reflect(-L, N));

  vec4 ambient = lightAmb * materialAmb;
  vec4 diffuse = clamp(max(dot(N, L) * lightDiff * materialDiff, 0.0), 0.0, 1.0);
  vec4 specular = clamp(lightSpec * materialSpec * pow(max(dot(R, E), 0.0), shininess) , 0.0, 1.0);

  // spot direction vector is from position to the middle of sphere
  vec4 spotDirection = normalize(spotPos - spotLookAtPnt);
  float cosA = max(dot(spotDirection, N), 0.0);
  float power = 0;
  if(cosA > 0.0) {
    float intensity = cosA;
    float angle = degrees(acos(cosA));
    if(angle < spotConeAngle) {
      power = pow(cosA, spotAngAtten);
    }
  }
  vec4 spotlight = vec4(power, power, power, 0);
  vec4 textureColor = textureCube(texCube, reflectionVector);
  vec4 objCol = (ambient + diffuse + specular + spotlight);

  gl_FragColor = vec4(mix(objCol, textureColor, 0.2));
}
