#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "classes/ErrorHandler.h"

static unsigned CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
static unsigned CompileShader(unsigned type, const std::string& sourceCode);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1080, 720, u8"Olá Mundo", NULL, NULL);
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
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    unsigned indexBuffer;
    GLCall(glGenBuffers(1, &indexBuffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, indexBuffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6*sizeof(float), positions, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0,2,GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    
    std::string vertexShaderSourceCode = 
        "#version 330 core\n\n"
        "layout(location = 0) in vec4 position;\n\n"
        "void main()\n"
        "{\n"
        "    gl_Position = position;"
        "}\n";
    std::string fragmentShaderSourceCode = 
        "#version 330 core\n\n"
        "layout(location = 0) out vec4 color;\n\n"
        "void main()\n"
        "{\n"
        "    color = vec4(0.0, 0.5, 0.25, 1.0);"
        "}\n";

    unsigned shader = CreateShader(vertexShaderSourceCode, fragmentShaderSourceCode);
    GLCall(glUseProgram(shader));
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteShader(shader);
    
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
