#include "pch.h"
#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh() :
	_vao{},
	_vertex_vbo{},
	_normal_vbo{},
	_texture_vbo{},
	_color_vbo{},
	_ibo{}
{
}

Mesh::~Mesh()
{
	OnRelease();
}

void Mesh::OnRelease()
{
	glDeleteBuffers(1, &_vertex_vbo);
	glDeleteBuffers(1, &_normal_vbo);
	glDeleteBuffers(1, &_texture_vbo);
	glDeleteBuffers(1, &_color_vbo);
	glDeleteBuffers(1, &_ibo);
	glDeleteVertexArrays(1, &_vao);
}

void Mesh::CreateBuffer()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vertex_vbo);
	glGenBuffers(1, &_normal_vbo);
	glGenBuffers(1, &_texture_vbo);
	glGenBuffers(1, &_color_vbo);
	glGenBuffers(1, &_ibo);
}

void Mesh::BindVAO()
{
	glBindVertexArray(_vao);
}

void Mesh::BindIndex()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, &_index);
}

void Mesh::CreateVertex2(std::shared_ptr<Shader>& _shader)
{
	BindVAO();

	CreateVBO(_vertex_vbo, &_vertex, _shader, "v_pos", 3);
	CreateVBO(_color_vbo, &_color, _shader, "v_color", 4);

	BindIndex();
}

void Mesh::CreateVertex3(std::shared_ptr<Shader>& _shader)
{
	BindVAO();

	CreateVBO(_vertex_vbo, &_vertex, _shader, "v_pos", 3);
	CreateVBO(_normal_vbo, &_normal, _shader, "v_normal", 3);
	CreateVBO(_texture_vbo, &_texture, _shader, "v_texture", 2);

	BindIndex();
}

template<typename T> requires requires {  std::is_arithmetic_v<T>; std::is_class_v<T>; }
void Mesh::CreateVBO(uint32_t vbo, const std::vector<T>* cont, std::shared_ptr<Shader>& _shader, const std::string& name, uint32_t count)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, cont);

	int32_t attrib{ glGetAttribLocation(_shader->ID(), name.c_str()) };
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib, count, GL_FLOAT, GL_FALSE, 0, 0);
}

template<typename T>
void Mesh::glBufferData(uint32_t target, const std::vector<T>* cont)
{
	::glBufferData(target, cont->size() * sizeof(T), cont->data(), GL_STATIC_DRAW);
}
