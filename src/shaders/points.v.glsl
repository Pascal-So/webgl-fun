#version 100

uniform mat4 viewmat;
uniform mat4 projmat;

attribute vec3 position;
varying vec3 col;

float v;

void main() {
	gl_Position = projmat * viewmat * vec4(position.xyz, 1.0);
	gl_PointSize = 15.;
	v = clamp(-0.05 * gl_Position.z + 1.2, 0., 1.);
	col = vec3(v*0.9,v*0.95,v);
}
