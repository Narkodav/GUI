#pragma once
#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string vertexSource, fragmentSource;
};

class Shader
{
private:
	unsigned int m_id;
	std::string m_filePath;
	mutable std::unordered_map<std::string, int> m_uniformLocationCache;
public:
	Shader();
	Shader(const std::string& filepath);
	~Shader();

	void set(const std::string& filepath);

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int value) const;
	void setUniform1f(const std::string& name, float value) const;
	void setUniform2f(const std::string& name, float v0, float v1) const;
	void setUniform3f(const std::string& name, float v0, float v1, float v2) const;
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix) const;
	int getUniformLocation(const std::string& name) const;

private:
	ShaderProgramSource parseShader(const std::string& filepath);
	unsigned int createShader(const ShaderProgramSource sources);
	unsigned int compileShader(unsigned int type, const std::string& source);
	
	
};

