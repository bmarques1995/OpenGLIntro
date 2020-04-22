#pragma once

#include <sstream>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "ErrorHandler.h"
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader 
{
private:
	std::string m_FilePath;
	unsigned m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const std::string& uniformName, float x, float y, float z, float w);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned CreateShader(const std::string& vertexShaderSourceCode, const std::string& fragmentShaderSourceCode);
	unsigned CompileShader(unsigned type, const std::string& sourceCode);
	int GetUniformLocation(const std::string& uniformName);
};