#pragma once

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

	void SetUniform1i(const std::string& uniformName, int x);
	void SetUniform1f(const std::string& uniformName, float x);
	void SetUniform4f(const std::string& uniformName, float x, float y, float z, float w);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned CreateShader(const std::string& vertexShaderSourceCode, const std::string& fragmentShaderSourceCode);
	unsigned CompileShader(unsigned type, const std::string& sourceCode);
	int GetUniformLocation(const std::string& uniformName);
};