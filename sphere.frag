varying vec3 fragment_position;   // The fragment position interpolated from the vertex shader
varying vec4 fragment_normal;   // The fragment normal interpolated from the vertex shader
varying vec4 vtx_normal;

void main() {
  gl_FragColor = vec4(vtx_normal.x, vtx_normal.y, vtx_normal.z, 0.0);
}
