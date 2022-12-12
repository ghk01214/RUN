#version 450

in vec3 f_pos;
in vec3 f_normal;
in vec2 f_texture;

out vec4 color;

uniform mat4 model;

uniform float ambient_strength;
uniform float specular_strength;
uniform int shininess;
uniform vec3 light_pos;
uniform vec3 light_color;

uniform vec3 view_pos;
uniform vec4 obj_color;

uniform bool light_on;

vec3 Ambient()
{	
	return ambient_strength * light_color;
}

vec3 Diffuse(vec3 normal, vec3 light_direction)
{
	return max(dot(normal, light_direction), 0.0) * light_color;
}

vec3 Specular(vec3 normal, vec3 light_direction)
{
	vec3 view_direction = normalize(view_pos - f_pos);
	vec3 reflect_direction = reflect(-light_direction, normal);
	float specular_light = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

	return specular_strength * specular_light * light_color;
}

vec3 Phong(vec3 normal, vec3 light_direction)
{
	return Ambient() + Diffuse(normal, light_direction) + Specular(normal, light_direction);
}

void main()
{
	vec3 normal = normalize(f_normal);
	vec3 light_direction = normalize(light_pos - f_pos);
	
	color = obj_color;
	
	if (light_on == true)
		color *= vec4(Phong(normal, light_direction), 1.0);
}