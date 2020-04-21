#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/classes/ErrorHandler.h"
#include <fstream>
#include <sstream>

static unsigned CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
static unsigned CompileShader(unsigned type, const std::string& sourceCode);

struct ShaderProgramSource 
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
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
    return {strStream[0].str(), strStream[1].str()};
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(720, 720, u8"Olá Mundo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
       -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f,  0.5f,
       -0.5f,  0.5f
    };

    unsigned indices[] = {
        0,1,2,
        2,3,0
    };

    unsigned buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW));

    unsigned indexBuffer;
    GLCall(glGenBuffers(1, &indexBuffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned), indices, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    
    ShaderProgramSource programSource = ParseShader("src/res/shaders/basic.shader");
    
    unsigned shader = CreateShader(programSource.VertexSource, programSource.FragmentSource);
    GLCall(glUseProgram(shader));
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    
    glfwTerminate();
    return 0;
}

unsigned CreateShader(const std::string& vertexShaderSourceCode, const std::string& fragmentShaderSourceCode)
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

unsigned CompileShader(unsigned type, const std::string& sourceCode)
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
        
        std::cout << "Failed to compile "<< 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<" shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        delete[] message;
        return 0;
    }


    return id;
}
