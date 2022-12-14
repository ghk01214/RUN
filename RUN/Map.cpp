#include "pch.h"
#include "Map.h"
#include <Shader.h>
#include <Rect.h>
#include <Cube.h>
#include <Teapot.h>
#include <algorithm>

Map::Map(std::shared_ptr<Shader>& shader, glm::vec4 color, glm::vec3 pos) :
	_tiles{},
	_lines{},
	_blocks(18, nullptr),
	_items(18, std::make_pair(nullptr, false))
{
	CreateTiles(&_tiles, color, shader);
	CreateTiles(&_lines, color, shader);

	for (auto& line : _lines)
	{
		line->TurnOffLight();
	}

	for (auto& tile : _tiles)
	{
		tile->Move(pos);
		tile->Transform(shader);
	}

	for (auto& line : _lines)
	{
		line->SetDrawType(GL_LINES);
		line->SetObjectColor(BLACK, 1.f);
		line->Scale(glm::vec3{ 0.999f });
		line->Move(pos);
		line->Transform(shader);
	}
}

Map::~Map()
{
	OnRelease();
}

void Map::OnLoad(std::shared_ptr<Shader>& shader)
{
	for (auto& obj : _tiles)
	{
		if (obj != nullptr)
			obj->OnLoad(shader);
	}

	for (auto& obj : _lines)
	{
		if (obj != nullptr)
			obj->OnLoad(shader);
	}

	for (auto& [obj, render] : _items)
	{
		if (obj != nullptr)
			obj->OnLoad(shader);
	}

	for (auto& obj : _blocks)
	{
		if (obj != nullptr)
			obj->OnLoad(shader);
	}
}

void Map::OnRelease()
{
	for (auto& tile : _tiles)
	{
		delete tile;
		tile = nullptr;
	}

	for (auto& tile : _lines)
	{
		delete tile;
		tile = nullptr;
	}

	for (auto& [item, render] : _items)
	{
		delete item;
		item = nullptr;
	}

	for (auto& block : _blocks)
	{
		delete block;
		block = nullptr;
	}
}

void Map::CreateTiles(std::vector<Rect*>* rect, glm::vec4 color, std::shared_ptr<Shader>& shader)
{
	rect->reserve(18);

	float scale_size{ 5.f };

	for (int32_t i = 0; i < 18; ++i)
	{
		rect->push_back(new Rect{});
		rect->back()->SetShader(shader);
		rect->back()->SetObjectColor(color);
		rect->back()->SetLightPos(-5.f, 5.f, -5.f);
		rect->back()->TurnOnLight();
		rect->back()->Scale(glm::vec3{ scale_size });
		rect->back()->RotateX(90.f);

		if (i % 3 == 0)
			rect->back()->Move(vec3::left(scale_size));
		else if (i % 3 == 2)
			rect->back()->Move(vec3::right(scale_size));
	}

	for (int32_t i = 3; i < 6; ++i)
	{
		(*rect)[i]->RotateZ(-60.f);
		(*rect)[i]->Move(glm::vec3{ -2.25f * scale_size, 1.299f * scale_size, 0.f });
	}

	for (int32_t i = 6; i < 9; ++i)
	{
		(*rect)[i]->RotateZ(60.f);
		(*rect)[i]->Move(glm::vec3{ -2.25f * scale_size, 1.299f * scale_size * 3, 0.f });
	}

	for (int32_t i = 9; i < 12; ++i)
	{
		(*rect)[i]->Move(vec3::up(1.299f * scale_size * 4));
	}

	for (int32_t i = 12; i < 15; ++i)
	{
		(*rect)[i]->RotateZ(-60.f);
		(*rect)[i]->Move(glm::vec3{ 2.25f * scale_size, 1.299f * scale_size * 3, 0.f });
	}

	for (int32_t i = 15; i < 18; ++i)
	{
		(*rect)[i]->RotateZ(60.f);
		(*rect)[i]->Move(glm::vec3{ 2.25f * scale_size, 1.299f * scale_size, 0.f });
	}
}

void Map::Move(glm::vec3 delta, glm::vec3 light_pos)
{
	for (auto& tile : _tiles)
	{
		if (tile == nullptr)
			continue;

		tile->SetLightPos(light_pos);
		tile->Move(delta);
	}

	for (auto& tile : _lines)
	{
		if (tile == nullptr)
			continue;

		tile->SetLightPos(light_pos);
		tile->Move(delta);
	}

	for (auto& [item, render] : _items)
	{
		if (item == nullptr)
			continue;

		item->SetLightPos(light_pos);
		item->Move(delta);
	}

	for (auto& block : _blocks)
	{
		if (block == nullptr)
			continue;

		block->SetLightPos(light_pos);
		block->Move(delta);
	}
}

void Map::Rotate(float delta, glm::vec3 light_pos)
{
	for (auto& tile : _tiles)
	{
		if (tile == nullptr)
			continue;

		tile->Move(vec3::down(1.299f * 2.f * 5.f));
		tile->RotateZ(delta);
		tile->Move(vec3::up(1.299f * 2.f * 5.f));
		tile->SetLightPos(light_pos);
	}

	for (auto& tile : _lines)
	{
		if (tile == nullptr)
			continue;

		tile->Move(vec3::down(1.299f * 2.f * 5.f));
		tile->RotateZ(delta);
		tile->Move(vec3::up(1.299f * 2.f * 5.f));
		tile->SetLightPos(light_pos);
	}

	for (auto& [item, render] : _items)
	{
		if (item == nullptr)
			continue;

		item->Move(vec3::down(1.299f * 2.f * 5.f));
		item->RotateZ(delta);
		item->Move(vec3::up(1.299f * 2.f * 5.f));
		item->SetLightPos(light_pos);
	}

	for (auto& block : _blocks)
	{
		if (block == nullptr)
			continue;

		block->Move(vec3::down(1.299f * 2.f * 5.f));
		block->RotateZ(delta);
		block->Move(vec3::up(1.299f * 2.f * 5.f));
		block->SetLightPos(light_pos);
	}
}

void Map::AddBlock(int32_t index, std::shared_ptr<Shader>& shader)
{
	_blocks[index] = new Cube{};
	_blocks[index]->SetShader(shader);
	_blocks[index]->SetObjectColor(0.4f, 0.294f, 0.f, 1.f);
	_blocks[index]->SetLightPos(-5.f, 5.f, -5.f);
	_blocks[index]->TurnOnLight();
	_blocks[index]->Scale(5.f, 3.f, 5.f);
	_blocks[index]->Move(vec3::up(0.5f));

	auto pos{ _tiles[index]->GetPos() };

	if (3 <= index and index < 6)
	{
		_blocks[index]->RotateZ(-60.f);
		_blocks[index]->Move(pos.x + 0.7f, pos.y, pos.z);
	}
	else if (6 <= index and index < 9)
	{
		_blocks[index]->RotateZ(-120.f);
		_blocks[index]->Move(pos.x + 0.7f, pos.y, pos.z);
	}
	else if (9 <= index and index < 12)
	{
		_blocks[index]->RotateZ(180.f);
		_blocks[index]->Move(pos.x, pos.y - 0.7f, pos.z);
	}
	else if (12 <= index and index < 15)
	{
		_blocks[index]->RotateZ(120.f);
		_blocks[index]->Move(pos.x - 0.7f, pos.y, pos.z);
	}
	else if (index >= 15)
	{
		_blocks[index]->RotateZ(60.f);
		_blocks[index]->Move(pos.x - 0.7f, pos.y, pos.z);
	}
	else
	{
		_blocks[index]->Move(pos.x, pos.y + 0.7f, pos.z);
	}
}

void Map::RemoveTile(int32_t index)
{
	delete _tiles[index];
	delete _lines[index];

	_tiles[index] = nullptr;
	_lines[index] = nullptr;
}

void Map::ChangeTileColor(int32_t index, float r, float g, float b, std::shared_ptr<Shader>& shader)
{
	_tiles[index]->SetObjectColor(r, g, b, 1.f);

	_items[index].first = new Teapot{};
	_items[index].second = true;
	_items[index].first->SetShader(shader);
	_items[index].first->SetObjectColor(r, g, b, 1.f);
	_items[index].first->SetLightPos(-5.f, 5.f, -5.f);
	_items[index].first->TurnOnLight();

	auto pos{ _tiles[index]->GetPos() };

	if (3 <= index and index < 6)
	{
		_items[index].first->RotateZ(-60.f);
		_items[index].first->Move(pos.x + 0.7f, pos.y, pos.z);
	}
	else if (6 <= index and index < 9)
	{
		_items[index].first->RotateZ(-120.f);
		_items[index].first->Move(pos.x + 0.7f, pos.y, pos.z);
	}
	else if (9 <= index and index < 12)
	{
		_items[index].first->RotateZ(180.f);
		_items[index].first->Move(pos.x, pos.y - 0.7f, pos.z);
	}
	else if (12 <= index and index < 15)
	{
		_items[index].first->RotateZ(120.f);
		_items[index].first->Move(pos.x - 0.7f, pos.y, pos.z);
	}
	else if (index >= 15)
	{
		_items[index].first->RotateZ(60.f);
		_items[index].first->Move(pos.x - 0.7f, pos.y, pos.z);
	}
	else
	{
		_items[index].first->Move(pos.x, pos.y + 0.7f, pos.z);
	}
}

void Map::Reuse(float delta, glm::vec3 light_pos)
{
	Move(vec3::front(delta), light_pos);

	for (auto& [item, render] : _items)
	{
		if (item == nullptr)
			continue;

		render = true;
	}
}

void Map::Render(std::shared_ptr<Shader>& shader, bool fever)
{
	for (auto& tile : _tiles)
	{
		if (tile == nullptr)
			continue;

		tile->BindVAO();
		tile->Transform(shader);
		tile->ApplyLight();

		glDrawElements(tile->GetDrawType(), tile->GetIndexNum(), GL_UNSIGNED_INT, 0);
	}

	for (auto& tile : _lines)
	{
		if (tile == nullptr)
			continue;

		tile->BindVAO();
		tile->Transform(shader);
		tile->ApplyLight();

		glDrawElements(tile->GetDrawType(), tile->GetIndexNum(), GL_UNSIGNED_INT, 0);
	}

	if (fever == true)
		return;

	for (auto& [item, render] : _items)
	{
		if (item == nullptr)
			continue;

		if (render == false)
			continue;

		item->BindVAO();
		item->Transform(shader);
		item->ApplyLight();

		glDrawElements(item->GetDrawType(), item->GetIndexNum(), GL_UNSIGNED_INT, 0);
	}

	for (auto& block : _blocks)
	{
		if (block == nullptr)
			continue;

		block->BindVAO();
		block->Transform(shader);
		block->ApplyLight();

		glDrawElements(block->GetDrawType(), block->GetIndexNum(), GL_UNSIGNED_INT, 0);
	}
}

const float Map::GetPos() const
{
	for (auto& tile : _tiles)
	{
		if (tile != nullptr)
			return tile->GetPos().z;
	}
}

void Map::SetLightPos(glm::vec3 pos)
{
	for (auto& tile : _tiles)
	{
		if (tile == nullptr)
			continue;

		tile->SetLightPos(pos);
	}

	for (auto& [item, render] : _items)
	{
		if (item == nullptr)
			continue;

		item->SetLightPos(pos);
	}

	for (auto& block : _blocks)
	{
		if (block == nullptr)
			continue;

		block->SetLightPos(pos);
	}
}

int32_t Map::CheckCollision(Object* other)
{
	if (other->GetPos().x + other->GetRadius().x > 7.5f)
		return 1;

	if (other->GetPos().x - other->GetRadius().x < -7.5f)
		return 2;

	if (other->GetPos().y - other->GetRadius().y < 0.f)
	{
		/*for (auto& tile : _tiles)
		{
			if (tile == nullptr)
				continue;

			if (tile->CheckCollision(other) == 0)
				return 3;
		}*/

		for (int32_t i = 0; i < _tiles.size(); ++i)
		{
			if (_tiles[i] == nullptr)
				continue;

			if (_tiles[i]->CheckCollision(other) == 0)
				return 3;
		}

		return 4;
	}

	return 0;
}

int32_t Map::CheckItemCollision(Object* player)
{
	for (auto& [item, render] : _items)
	{
		if (item == nullptr)
			continue;

		if (render == false)
			continue;

		if (item->CheckCollision(player) == 0)
		{
			render = false;

			if (item->GetColor() == glm::vec3{ RED })
				return 1;
			else if (item->GetColor() == glm::vec3{ GREEN })
				return 2;
			else if (item->GetColor() == glm::vec3{ BLUE })
				return 3;
		}
	}
}

bool Map::CheckBlockCollision(Object* player)
{
	for (auto& block : _blocks)
	{
		if (block == nullptr)
			continue;

		if (block->CheckCollision(player) == 0)
			return true;
	}

	return false;
}
