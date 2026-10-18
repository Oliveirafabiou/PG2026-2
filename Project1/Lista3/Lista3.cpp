#include "Lista3.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Lista3 {

    // Estrutura para armazenar a cor RGB de um triângulo
    struct Cor {
        float r, g, b;
    };

    // Variáveis globais/estáticas de controle da sessão
    static std::vector<float> g_Vertices;   // Guarda as posições (x, y, z) de todos os vértices
    static std::vector<Cor> g_CoresTriangulos; // Guarda a cor de cada triângulo (1 cor para cada 3 vértices)
    static Cor g_CorAtual = { 1.0f, 0.0f, 0.0f }; // Cor padrão inicial (Vermelho)
    static GLuint g_VBO = 0;
    static GLuint g_VAO = 0;

    // Gera uma cor aleatória para o próximo triângulo
    static Cor gerarCorAleatoria() {
        Cor c;
        c.r = (float)(rand() % 100) / 100.0f;
        c.g = (float)(rand() % 100) / 100.0f;
        c.b = (float)(rand() % 100) / 100.0f;
        return c;
    }

    // Callback de evento de clique do mouse
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            int width, height;
            glfwGetWindowSize(window, &width, &height);

            // Conversão de coordenadas da janela (Pixels) para NDC (-1.0 a 1.0)
            float xNDC = (2.0f * (float)xpos) / (float)width - 1.0f;
            float yNDC = 1.0f - (2.0f * (float)ypos) / (float)height;

            // Adiciona o novo vértice
            g_Vertices.push_back(xNDC);
            g_Vertices.push_back(yNDC);
            g_Vertices.push_back(0.0f);

            std::cout << "Vértice criado (" << (g_Vertices.size() / 3) << "): ("
                << xNDC << ", " << yNDC << ")\n";

            // Atualiza a cor se estarmos no primeiro vértice de um novo triângulo
            if ((g_Vertices.size() / 3) % 3 == 1 && g_Vertices.size() > 3) {
                g_CorAtual = gerarCorAleatoria();
            }

            // Se completou 3 vértices, registra a cor do novo triângulo
            if ((g_Vertices.size() / 3) % 3 == 0) {
                g_CoresTriangulos.push_back(g_CorAtual);
                std::cout << "-> Novo triângulo criado com sucesso!\n";
            }

            // Atualiza os dados no VBO da GPU
            glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
            glBufferData(GL_ARRAY_BUFFER, g_Vertices.size() * sizeof(float), g_Vertices.data(), GL_DYNAMIC_DRAW);
        }
    }

    // Helper interno para compilar os shaders
    static GLuint compileShaderHelper() {
        std::string vert_sdr = "#version 330 core\n"
            "layout (location = 0) in vec3 position;\n"
            "void main(){ gl_Position = vec4(position, 1.0); }";
        std::string frag_sdr = "#version 330 core\n"
            "uniform vec4 tint;\n"
            "out vec4 result;\n"
            "void main(){ result = tint; }";

        GLuint vid = glCreateShader(GL_VERTEX_SHADER);
        const char* vs_ptr = vert_sdr.c_str();
        glShaderSource(vid, 1, &vs_ptr, NULL);
        glCompileShader(vid);

        GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fs_ptr = frag_sdr.c_str();
        glShaderSource(fid, 1, &fs_ptr, NULL);
        glCompileShader(fid);

        GLuint program = glCreateProgram();
        glAttachShader(program, vid);
        glAttachShader(program, fid);
        glLinkProgram(program);

        glDeleteShader(vid);
        glDeleteShader(fid);

        return program;
    }

    void run() {
        // Semente para geração de cores aleatórias
        srand((unsigned int)time(NULL));

        // Limpa dados acumulados de execuções anteriores
        g_Vertices.clear();
        g_CoresTriangulos.clear();
        g_CorAtual = { 1.0f, 0.0f, 0.0f }; // Reseta para vermelho no primeiro triângulo

        if (!glfwInit()) return;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(800, 600, "Lista 3 - Criar Triângulos com Clique", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) return;

        // Registra a função callback de clique do mouse
        glfwSetMouseButtonCallback(window, mouseButtonCallback);

        GLuint programID = compileShaderHelper();
        GLint colorLoc = glGetUniformLocation(programID, "tint");

        // Configuração inicial do VAO/VBO
        glGenVertexArrays(1, &g_VAO);
        glBindVertexArray(g_VAO);

        glGenBuffers(1, &g_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glPointSize(8.0f); // Aumenta os pontos para destacar vértices isolados

        std::cout << "\n=============================================\n";
        std::cout << "Instruções Lista 3:\n";
        std::cout << "- Clique na janela para inserir vértices.\n";
        std::cout << "- A cada 3 cliques, um triângulo será formado.\n";
        std::cout << "- Cada novo triângulo terá uma cor diferente.\n";
        std::cout << "=============================================\n";

        while (!glfwWindowShouldClose(window)) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(programID);
            glBindVertexArray(g_VAO);

            int totalVertices = (int)g_Vertices.size() / 3;
            int numTriangulosCompletos = totalVertices / 3;
            int verticesSobra = totalVertices % 3;

            // 1. Renderiza os triângulos já formados, cada um com sua cor
            for (int i = 0; i < numTriangulosCompletos; ++i) {
                Cor c = g_CoresTriangulos[i];
                glUniform4f(colorLoc, c.r, c.g, c.b, 1.0f);
                glDrawArrays(GL_TRIANGLES, i * 3, 3);
            }

            // 2. Desenha os vértices pendentes (1 ou 2 cliques) como pontos na cor do próximo triângulo
            if (verticesSobra > 0) {
                glUniform4f(colorLoc, g_CorAtual.r, g_CorAtual.g, g_CorAtual.b, 1.0f);
                glDrawArrays(GL_POINTS, numTriangulosCompletos * 3, verticesSobra);
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &g_VAO);
        glDeleteBuffers(1, &g_VBO);
        glDeleteProgram(programID);
        glfwTerminate();
    }

} // namespace Lista3
