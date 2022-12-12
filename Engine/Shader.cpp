#include "pch.h"
#include <sstream>
#include "Shader.h"

Shader::Shader() :
	_id{ glCreateProgram() }
{
}

Shader::Shader(const std::string& vertex, const std::string& fragment)
{
	OnLoad(vertex, fragment);
}

Shader::~Shader()
{
}

bool Shader::Add(const std::string& path, uint32_t type)
{
	uint32_t shader{ glCreateShader(type) };

	if (shader == 0)
	{
		std::cout << std::format("Error creating shader type\n");
		return false;
	}

	std::string code{ Util::ReadFile(path) };

	if (code.empty())
	{
		std::cout << std::format("Error compiling {} shader\n", path);
		return false;
	}

	auto source{ code.data() };
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	int32_t success{};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (_FAILED success)
	{
		std::string log{};
		log.resize(1024);

		glGetShaderInfoLog(shader, log.size(), nullptr, log.data());
		std::cout << std::format("Error compiling shader type {} : {}\n{}", type, log, code);

		return false;
	}

	glAttachShader(_id, shader);
	glDeleteShader(shader);

	return true;
}

int Shader::Link()
{
	int32_t success{};
	std::string log{};
	log.resize(1024);

	glLinkProgram(_id);
	glGetProgramiv(_id, GL_LINK_STATUS, &success);

	if (_FAILED success)
	{
		glGetProgramInfoLog(_id, log.size(), nullptr, log.data());

		return 1;
	}

	success = 0;
	log.clear();
	log.resize(1024);

	glValidateProgram(_id);
	glGetProgramiv(_id, GL_VALIDATE_STATUS, &success);

	if (_FAILED success)
	{
		glGetProgramInfoLog(_id, log.size(), nullptr, log.data());

		return 2;
	}

	return 0;
}

void Shader::Compile(const std::string& type, ...)
{
	std::stringstream attribute;
	attribute.str(type);

	std::string path;
	uint32_t shader_type{};

	int32_t count{};
	attribute >> count;

	for (int32_t i = 0; i < count; ++i)
	{
		attribute >> path >> shader_type;

		if (Add(path, shader_type) == false)
		{
			std::cout << std::format("Error attaching {} shader", path);
			return;
		}
	}

	int32_t link{ Link() };

	if (link == 1)
	{
		std::cout << std::format("Error linking shader program\n");
		return;
	}

	if (link == 2)
	{
		std::cout << std::format("Error valid shader program\n");
		return;
	}

	std::cout << std::format("Shader compiling is done\n");
	OnUse();
}

void Shader::OnLoad(const std::string& vertex, const std::string& fragment)
{
	std::string str{ "2 " };
	str += vertex;
	str += " ";
	str += std::to_string(GL_VERTEX_SHADER);
	str += " ";
	str += fragment;
	str += " ";
	str += std::to_string(GL_FRAGMENT_SHADER);

	Compile(str);
}

void Shader::OnUse()
{
	glUseProgram(_id);
}

void Shader::SetBool(const std::string& name, bool value)
{
	SetInt(name, Convert::ToInt32(value));
}

void Shader::SetInt(const std::string& name, int32_t value)
{
	auto location{ glGetUniformLocation(_id, name.c_str()) };
	glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& name, float value)
{
	auto location{ glGetUniformLocation(_id, name.c_str()) };
	glUniform1f(location, value);
}

void Shader::SetVec2(const std::string& name, glm::vec3* value_ptr)
{
	auto location{ glGetUniformLocation(_id, name.c_str()) };
	glUniform2fv(location, 1, glm::value_ptr(*value_ptr));
}

void Shader::SetVec3(const std::string& name, glm::vec3* value_ptr)
{
	auto location{ glGetUniformLocation(_id, name.c_str()) };
	glUniform3fv(location, 1, glm::value_ptr(*value_ptr));
}

void Shader::SetVec4(const std::string& name, glm::vec4* value_ptr)
{
	auto location{ glGetUniformLocation(_id, name.c_str()) };
	glUniform4fv(location, 1, glm::value_ptr(*value_ptr));
}

void Shader::SetMat4(const std::string& name, glm::mat4* value_ptr)
{
	auto location{ glGetUniformLocation(_id, name.c_str()) };
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(*value_ptr));
}
