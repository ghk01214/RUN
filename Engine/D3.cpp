#include "pch.h"
#include "D3.h"

D3::D3(const std::string& path, glm::vec3 pos)
{
	std::vector<float> vertex;
	std::vector<float> normal;
	std::vector<float> texture;
	std::vector<uint32_t> index;

	// obj 파일에서 vertex, normal, texture 좌표 값 및 index 값을 읽어와서 저장
	Util::ReadModel(path, &vertex, &normal, &texture, &index);

	// 읽어들인 vertex, normal, texture 좌표와 index 값을 mesh에 저장
	_mesh->SetVertex(&vertex);
	_mesh->SetNormal(&normal);
	_mesh->SetTexture(&texture);
	_mesh->SetIndex(&index);

	// 인자로 받아들인 위치로 객체 이동
	Move(pos);
}

void D3::OnLoad(std::shared_ptr<Shader>& shader)
{
	Object::OnLoad(shader);

	_mesh->CreateVertex3(shader);
}
