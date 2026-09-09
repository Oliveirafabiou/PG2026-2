#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>

// COMPILAÇÃO DOS SHADERS
GLuint shaderProgram(std::string const& vs, std::string const& fs) {
    char logStr[1024] = "";

    GLuint vid = glCreateShader(GL_VERTEX_SHADER);
    char const* vs_ptr = vs.c_str();
    glShaderSource(vid, 1, &vs_ptr, NULL);
    glCompileShader(vid);

    GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
    char const* fs_ptr = fs.c_str();
    glShaderSource(fid, 1, &fs_ptr, NULL);
    glCompileShader(fid);

    GLuint program = glCreateProgram();
    glAttachShader(program, vid);
    glAttachShader(program, fid);
    glLinkProgram(program);

    glGetProgramInfoLog(program, 1023, NULL, logStr);
    std::cout << logStr << std::endl;

    glDetachShader(program, vid);
    glDetachShader(program, fid);
    glDeleteShader(vid);
    glDeleteShader(fid);

    return program;
}

int main(void) {
    // Inicialização do GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Draw triangulos - 3 formas juntas", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Inicialização da GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // SHADERS
    std::string vert_sdr = R"str_limiter(
    #version 330 core
    
    layout (location = 0) in vec3 position;
    
    void main(){
        gl_Position = vec4(position, 1.0);
    }
    )str_limiter";

    std::string frag_sdr = R"limiter(
    #version 330 core

    uniform vec4 tint;
    out vec4 result;

    void main(){
        result = tint;
    }
    )limiter";

    GLuint programID = shaderProgram(vert_sdr, frag_sdr);
    glUseProgram(programID);

    GLint colorLoc = glGetUniformLocation(programID, "tint");
    assert(colorLoc > -1);

    // Vértices (Gravatinha de triângulos)
    float vertices[] = {
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.0f,  0.0f, 0.0f,
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    int numVertices = sizeof(vertices) / sizeof(float) / 3;

    // Ajusta o tamanho dos pontos para ficarem visíveis junto com o desenho
    glPointSize(8.0f);
    // Ajusta a grossura das linhas de contorno (opcional, dependendo do suporte da placa)
    glLineWidth(2.0f);

    // Loop principal
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Cor de fundo cinza escuro para contraste
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);
        glBindVertexArray(VAO);

        // 1. Desenha o interior preenchido (Cor: Vermelho)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);

        // 2. Desenha o contorno por cima (Cor: Verde)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);

        // 3. Desenha os vértices como pontos por cima de tudo (Cor: Azul)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Garante modo normal para pontos
        glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_POINTS, 0, numVertices);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza de recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programID);

    glfwTerminate();
    return 0;
}