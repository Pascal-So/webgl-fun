#version 100

precision lowp float;
varying vec3 col;

float alpha;

void main() {
	vec2 uv = gl_PointCoord.xy - vec2(0.5);
	float sqrad = uv.x * uv.x + uv.y * uv.y;
	alpha = 1.; // clamp(4. * (0.25 - sqrad) + 0.5, 0., 1.);
	gl_FragColor = vec4(1., 1., 1., 0.3);
}
