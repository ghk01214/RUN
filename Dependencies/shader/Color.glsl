#version 450

out vec4 color;

uniform vec3 obj_color;

void main()
{
	color = vec4(obj_color, 1.0);
}