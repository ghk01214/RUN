#pragma once

// NOTE :
// 월드 상에 그려질 object들을 구성하는 mesh class
// 

class Shader;

class Mesh
{
public:
	Mesh();
	~Mesh();

	void OnRelease();

	void CreateBuffer();
	void BindVAO();
	void BindIndex();

	void CreateVertex2(std::shared_ptr<Shader>& _shader);
	void CreateVertex3(std::shared_ptr<Shader>& _shader);

	size_t GetIndexNum() { return _index.size(); }

	void SetVertex(std::vector<float>* vertex) { _vertex = *vertex; }
	void SetNormal(std::vector<float>* normal) { _normal = *normal; }
	void SetTexture(std::vector<float>* texture) { _texture = *texture; }
	void SetColor(std::vector<float>* color) { _color = *color; }
	void SetIndex(std::vector<uint32_t>* index) { _index = *index; }

private:
	template<typename T> requires requires {  std::is_arithmetic_v<T>; std::is_class_v<T>; }
	void CreateVBO(uint32_t vbo, const std::vector<T>* cont, std::shared_ptr<Shader>& _shader, const std::string& name, uint32_t count);

	template<typename T>
	void glBufferData(uint32_t target, const std::vector<T>* cont);

private:
	uint32_t _vao;
	
	uint32_t _vertex_vbo;
	std::vector<float> _vertex;

	uint32_t _normal_vbo;
	std::vector<float> _normal;

	uint32_t _texture_vbo;
	std::vector<float> _texture;

	uint32_t _color_vbo;
	std::vector<float> _color;

	uint32_t _ibo;
	std::vector<uint32_t> _index;
};
