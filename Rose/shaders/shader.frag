#version 330 core
out vec4 FragColor;
in vec3 color;
uniform float time;
void main()
{
	vec3 rainbow = vec3(
		sin(time + color.r * 6.28) * 0.5 + 0.5,
		sin(time + color.g * 6.28 + 2.0) * 0.5 + 0.5,
		sin(time + color.b * 6.28 + 4.0) * 0.5 + 0.5
	);
	FragColor = vec4(mix(color, rainbow, 0.7), 1.0f);
}
