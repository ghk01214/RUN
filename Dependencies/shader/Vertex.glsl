#version 450

in vec3 v_pos;
in vec3 v_normal;
in vec2 v_texture;

out vec3 f_pos;
out vec3 f_normal;
out vec2 f_texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(v_pos, 1.0);

	f_pos = vec3(model * vec4(v_pos, 1.0));
	// normal 값을 새로 계산해서 빛의 위치가 바뀜에 따라 객체에 적용된 빛도 새로 계산 및 적용
	f_normal = mat3(transpose(inverse(model))) * v_normal;
	f_texture = v_texture;
}