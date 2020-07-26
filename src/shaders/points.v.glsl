#version 100

uniform mat4 viewmat;
uniform mat4 projmat;
attribute vec3 position;
varying vec3 col;

float v;

void main() {
	gl_Position = projmat * viewmat * vec4(position.xyz, 1.0);
	gl_PointSize = 1.;
	v = -0.4 * gl_Position.z + 1.2;
	col = vec3(v,v,v);
}
