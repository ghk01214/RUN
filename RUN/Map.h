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

	void Move(glm::vec3 delta);
	void Move(float x, float y, float z) { Move(glm::vec3{ x, y, z }); }
	void Rotate(float delta);

	void RemoveTile(int32_t index);
	void ChangeTileColor(int32_t index, float r, float g, float b, std::shared_ptr<Shader>& shader);
	void Reuse(float delta);

	void Render(std::shared_ptr<Shader>& shader);

	const float GetPos() const;

	int32_t CheckCollision(Object* other);
	void CheckItemCollision(Object* player);

private:
	std::vector<Rect*> _tiles;
	std::vector<Rect*> _lines;
	std::vector<class Cube*> _blocks;
	std::vector<std::pair<class Teapot*, bool>> _items;
};

