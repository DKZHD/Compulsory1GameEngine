#pragma once
#include <string>
#include <utility>

class Shader
{
	static std::pair<std::string, std::string>LoadShadersFromFile(const char* shader_source_path);

public:
	unsigned int Program;
	Shader(const char* shader_source_path);
	Shader() = default;
};

