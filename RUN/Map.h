#pragma once

class Shader;
class Object;
class Rect;

class Map
{
public:
	Map(std::shared_ptr<Shader>& shader, glm::vec4 color, glm::vec3 pos = vec3::zero());
	~Map();

	void OnLoad(std::shared_ptr<Shader>& shader);
	void OnRelease();

	void CreateTiles(std::vector<Rect*>* rect, glm::vec4 color, std::shared_ptr<Shader>& shader);

	void Move(glm::vec3 delta, glm::vec3 light_pos);
	void Move(float x, float y, float z, glm::vec3 light_pos) { Move(glm::vec3{ x, y, z }, light_pos); }
	void Rotate(float delta, glm::vec3 light_pos);

	void AddBlock(int32_t index, std::shared_ptr<Shader>& shader);
	void RemoveTile(int32_t index);
	void ChangeTileColor(int32_t index, float r, float g, float b, std::shared_ptr<Shader>& shader);
	void Reuse(float delta, glm::vec3 light_pos);

	void Render(std::shared_ptr<Shader>& shader, bool fever);

	const float GetPos() const;

	void SetLightPos(glm::vec3 pos);

	int32_t CheckCollision(Object* other);
	int32_t CheckItemCollision(Object* player);
	bool CheckBlockCollision(Object* player);

private:
	std::vector<Rect*> _tiles;
	std::vector<Rect*> _lines;
	std::vector<class Cube*> _blocks;
	std::vector<std::pair<class Teapot*, bool>> _items;

	glm::vec3 _pos;
};
