#include "Shader.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath)
{
	ShaderProgramSource programSource = ParseShader("src/res/shaders/basic.shader");
	m_RendererID = CreateShader(programSource.VertexSource, programSource.FragmentSource);

}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1d(const std::string& name, int x)
{
	GLCall(glUniform1i(GetUniformLocation(uniformName), x));
}

void Shader::SetUniform1f(const std::string& name, float x)
{
	GLCall(glUniform1f(GetUniformLocation(uniformName), x));
}

void Shader::SetUniform4f(const std::string& uniformName, float x, float y, float z, float w)
{

	GLCall(glUniform4f(GetUniformLocation(uniformName), x, y, z, w));
}

int Shader::GetUniformLocation(const std::string& uniformName)
{
	//performance
	if (m_UniformLocationCache.find(uniformName) != m_UniformLocationCache.end())
		return m_UniformLocationCache[uniformName];
	GLCall(int location = glGetUniformLocation(m_RendererID, uniformName.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform \'" << uniformName << "\' doesn\'t exist!" << std::endl;
	m_UniformLocationCache[uniformName] = location;
	return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream fileStream(filepath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::stringstream strStream[2];
	std::string line;
	ShaderType type = ShaderType::NONE;
	while (getline(fileStream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			strStream[(unsigned)type] << line << "\n";
		}
	}
	return { strStream[0].str(), strStream[1].str() };
}

unsigned Shader::CreateShader(const std::string& vertexShaderSourceCode, const std::string& fragmentShaderSourceCode)
{
	GLCall(unsigned program = glCreateProgram());
	GLCall(unsigned vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSourceCode));
	GLCall(unsigned fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSourceCode));

	GLCall(glAttachShader(program, vertexShader));
	GLCall(glAttachShader(program, fragmentShader));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	return program;
}

unsigned Shader::CompileShader(unsigned type, const std::string& sourceCode)
{
	GLCall(unsigned id = glCreateShader(type));
	const char* srcCode = sourceCode.c_str();
	GLCall(glShaderSource(id, 1, &srcCode, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int lengthPreProcessed;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lengthPreProcessed));

		size_t length = lengthPreProcessed;

		char* message = new char[length];
		GLCall(glGetShaderInfoLog(id, lengthPreProcessed, &lengthPreProcessed, message));

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		delete[] message;
		return 0;
	}

	return id;
}