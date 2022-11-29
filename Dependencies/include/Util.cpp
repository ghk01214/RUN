#include "pch.h"
#include <fstream>
#include <sstream>
#include <map>
#include <numeric>
#include "Util.h"

namespace Util
{
	template<class T>
	bool CheckSame(T& container, int32_t index_0, int32_t index_1, int32_t count)
	{
		for (int32_t i = 0; i < count; ++i)
		{
			if (container[index_0 * count + i] != container[index_1 * count + i])
				return false;
		}

		return true;
	}

	bool ReadFile(const std::string& path, std::string* blob)
	{
		std::ifstream file{ path };
		std::stringstream data{};

		if (file.fail())
		{
			std::cout << std::format("{} file load failed...\n", path);
			return false;
		}

		data << file.rdbuf();
		*blob = data.str();

		return true;
	}

	std::string ReadFile(const std::string& path)
	{
		std::ifstream file{ path };
		std::stringstream data{};

		if (file.fail())
		{
			std::cout << std::format("{} file load failed...\n", path);
			return std::string{};
		}

		data << file.rdbuf();

		return data.str();
	}

	bool ReadModel(const std::string& path, std::vector<float>* vertex, std::vector<float>* normal, std::vector<float>* texture, std::vector<uint32_t>* index)
	{
		std::ifstream file{ path };
		std::string line;
		std::stringstream stream;

		std::vector<float> in_ver{};
		std::vector<float> in_nor{};
		std::vector<float> in_tex{};

		std::vector<uint32_t> ver_index{};
		std::vector<uint32_t> nor_index{};
		std::vector<uint32_t> tex_index{};

		while (std::getline(file, line))
		{
			stream.clear();
			stream.str(line);

			std::string type;
			stream >> type;

			if (type == "#" or type.empty() == true)
				continue;

			if (type == "v")
			{
				glm::vec3 pos;
				stream >> pos.x >> pos.y >> pos.z;

				in_ver.push_back(pos.x);
				in_ver.push_back(pos.y);
				in_ver.push_back(pos.z);
			}
			else if (type == "vn")
			{
				glm::vec3 pos;
				stream >> pos.x >> pos.y >> pos.z;

				in_nor.push_back(pos.x);
				in_nor.push_back(pos.y);
				in_nor.push_back(pos.z);
			}
			else if (type == "vt")
			{
				glm::vec2 pos;
				stream >> pos.x >> pos.y;

				in_tex.push_back(pos.x);
				in_tex.push_back(pos.y);
			}
			else if (type == "f")
			{
				for (int32_t i = 0; i < 3; ++i)
				{
					std::string info;
					stream >> info;

					std::istringstream istream{ info };

					std::getline(istream, info, '/');
					ver_index.push_back(std::atoi(info.c_str()) - 1);

					std::getline(istream, info, '/');
					tex_index.push_back(std::atoi(info.c_str()) - 1);

					std::getline(istream, info, '/');
					nor_index.push_back(std::atoi(info.c_str()) - 1);
				}
			}
		}

		std::vector<float> ver_buff{};
		std::vector<float> nor_buff{};
		std::vector<float> tex_buff{};
		std::vector<int32_t> in_buff{};

		for (int32_t i = 0; i < ver_index.size(); ++i)
		{
			ver_buff.push_back(in_ver[ver_index[i] * 3 + 0]);
			ver_buff.push_back(in_ver[ver_index[i] * 3 + 1]);
			ver_buff.push_back(in_ver[ver_index[i] * 3 + 2]);

			nor_buff.push_back(in_nor[nor_index[i] * 3 + 0]);
			nor_buff.push_back(in_nor[nor_index[i] * 3 + 1]);
			nor_buff.push_back(in_nor[nor_index[i] * 3 + 2]);

			tex_buff.push_back(in_tex[tex_index[i] * 2 + 0]);
			tex_buff.push_back(in_tex[tex_index[i] * 2 + 1]);

			in_buff.push_back(i);
		}


		std::vector<int32_t> noDupLocations;
		noDupLocations.resize(in_buff.size());
		std::iota(noDupLocations.begin(), noDupLocations.end(), 0);

		std::map<int32_t, int32_t> oldIndexToNewIndexDict;

		int checkIndex = 0;
		do
		{
			for (int i = checkIndex + 1; i < (int)noDupLocations.size(); ++i)
			{
				if (!CheckSame(ver_buff, noDupLocations[checkIndex], noDupLocations[i], 3))
					continue;

				if (!CheckSame(nor_buff, noDupLocations[checkIndex], noDupLocations[i], 3))
					continue;

				if (!CheckSame(tex_buff, noDupLocations[checkIndex], noDupLocations[i], 2))
					continue;

				oldIndexToNewIndexDict.emplace(noDupLocations[i], checkIndex);
				noDupLocations.erase(noDupLocations.begin() + i);

				--i;
			}

			++checkIndex;
		} while (checkIndex < (int)noDupLocations.size());

		for (int i = 0; i < (int)noDupLocations.size(); ++i)
			oldIndexToNewIndexDict.emplace(noDupLocations[i], i);

		int32_t vertex_num{ Convert::ToInt32(noDupLocations.size()) };
		int32_t index_num{ Convert::ToInt32(ver_index.size()) };
		vertex->resize(vertex_num * 3);
		normal->resize(vertex_num * 3);
		texture->resize(vertex_num * 2);
		index->resize(index_num);

		for (int i = 0; i < vertex_num; ++i)
		{
			(*vertex)[i * 3 + 0] = ver_buff[noDupLocations[i] * 3 + 0];
			(*vertex)[i * 3 + 1] = ver_buff[noDupLocations[i] * 3 + 1];
			(*vertex)[i * 3 + 2] = ver_buff[noDupLocations[i] * 3 + 2];

			(*normal)[i * 3 + 0] = nor_buff[noDupLocations[i] * 3 + 0];
			(*normal)[i * 3 + 1] = nor_buff[noDupLocations[i] * 3 + 1];
			(*normal)[i * 3 + 2] = nor_buff[noDupLocations[i] * 3 + 2];

			(*texture)[i * 2 + 0] = tex_buff[noDupLocations[i] * 2 + 0];
			(*texture)[i * 2 + 1] = tex_buff[noDupLocations[i] * 2 + 1];
		}


		for (int i = 0; i < index_num; ++i)
		{
			(*index)[i] = oldIndexToNewIndexDict[in_buff[i]];
		}

		return true;
	}
}
