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
	_items(18, nullptr)
{
	CreateTiles(&_tiles, color, shader);
	CreateTiles(&_lines, color, shader);

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

	for (auto& obj : _items)
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

	for (auto& item : _items)
	{
		delete item;
		item = nullptr;
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

void Map::Move(glm::vec3 delta)
{
	for (auto& tile : _tiles)
	{
		if (tile != nullptr)
			tile->Move(delta);
	}

	for (auto& tile : _lines)
	{
		if (tile != nullptr)
			tile->Move(delta);
	}

	for (auto& item : _items)
	{
		if (item != nullptr)
			item->Move(delta);
	}
}

void Map::Rotate(float delta)
{
	for (auto& tile : _tiles)
	{
		if (tile != nullptr)
			tile->RotateZ(delta);
	}

	for (auto& tile : _lines)
	{
		if (tile != nullptr)
			tile->RotateZ(delta);
	}

	for (auto& item : _items)
	{
		if (item != nullptr)
			item->RotateZ(delta);
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

	_items[index] = new Teapot{};
	_items[index]->SetShader(shader);
	_items[index]->SetObjectColor(WHITE, 1.f);

	if (3 <= index and index < 6)
		_items[index]->RotateZ(-60.f);
	else if (6 <= index and index < 9)
		_items[index]->RotateZ(-120.f);
	else if (9 <= index and index < 12)
		_items[index]->RotateZ(180.f);
	else if (12 <= index and index < 15)
		_items[index]->RotateZ(120.f);
	else if (index >= 15)
		_items[index]->RotateZ(60.f);
		
	//_items[index]->Move(vec3::up(2.f));
	_items[index]->Move(_tiles[index]->GetPos());
}

void Map::Render(std::shared_ptr<Shader>& shader)
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

	for (auto& item : _items)
	{
		if (item == nullptr)
			continue;

		item->BindVAO();
		item->Transform(shader);
		item->ApplyLight();

		glDrawElements(item->GetDrawType(), item->GetIndexNum(), GL_UNSIGNED_INT, 0);
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

int32_t Map::CheckCollision(Object* other)
{
	//if (other->GetPos().x + other->GetRadius().x > GetPos().x + _radius.x)
	//	return 1;

	//if (other->GetPos().x - other->GetRadius().x < GetPos().x - _radius.x)
	//	return 2;

	//if ((other->GetPos().y + other->GetRadius().y > GetPos().y + _radius.y)
	//	or (other->GetPos().y - other->GetRadius().y < GetPos().y - _radius.y))
	//	return 3;

	//if ((other->GetPos().z + other->GetRadius().z > GetPos().z + _radius.z)
	//	or (other->GetPos().z - other->GetRadius().z < GetPos().z - _radius.z))
	//	return 4;

	return 0;
}
