#include "Lista3.h"
#include "../core/window.h"
#include "../core/shader.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Lista3 {

    struct Cor {
        float r, g, b;
    };

    // Estrutura para armazenar a animação e transformação individual de cada triângulo
    struct TrianguloAnimado {
        std::vector<float> verticesLocais; // 3 vértices normalizados em relação ao seu centro
        Cor cor;
        glm::vec2 pos;         // Posição do centro no espaço NDC (-1.0 a 1.0)
        glm::vec2 vel;         // Velocidade de movimento
        float angulo;          // Ângulo de rotação atual em graus
        float velRotacao;      // Velocidade angular (graus/frame)
    };

    static std::vector<float> g_PontosPendentes; // Vértices do triângulo ainda em formação (cliques 1 e 2)
    static std::vector<TrianguloAnimado> g_Triangulos;
    static Cor g_CorAtual = { 1.0f, 0.0f, 0.0f };
    static GLuint g_VAOPendentes = 0, g_VBOPendentes = 0;
    static GLuint g_VAOTriangulo = 0, g_VBOTriangulo = 0;

    static Cor gerarCorAleatoria() {
        return {
            (float)(rand() % 100) / 100.0f,
            (float)(rand() % 100) / 100.0f,
            (float)(rand() % 100) / 100.0f
        };
    }

    static float gerarFloatAleatorio(float min, float max) {
        return min + (float)rand() / ((float)RAND_MAX / (max - min));
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            int width, height;
            glfwGetWindowSize(window, &width, &height);

            // Conversão para NDC
            float xNDC = (2.0f * (float)xpos) / (float)width - 1.0f;
            float yNDC = 1.0f - (2.0f * (float)ypos) / (float)height;

            g_PontosPendentes.push_back(xNDC);
            g_PontosPendentes.push_back(yNDC);
            g_PontosPendentes.push_back(0.0f);

            // Se o primeiro ponto de um novo triângulo foi clicado, sorteia uma nova cor
            if (g_PontosPendentes.size() / 3 == 1 && !g_Triangulos.empty()) {
                g_CorAtual = gerarCorAleatoria();
            }

            // Atualiza os pontos pendentes na GPU
            glBindBuffer(GL_ARRAY_BUFFER, g_VBOPendentes);
            glBufferData(GL_ARRAY_BUFFER, g_PontosPendentes.size() * sizeof(float), g_PontosPendentes.data(), GL_DYNAMIC_DRAW);

            // Se completou 3 cliques, cria o triângulo animado
            if (g_PontosPendentes.size() / 3 == 3) {
                float cx = (g_PontosPendentes[0] + g_PontosPendentes[3] + g_PontosPendentes[6]) / 3.0f;
                float cy = (g_PontosPendentes[1] + g_PontosPendentes[4] + g_PontosPendentes[7]) / 3.0f;

                TrianguloAnimado tri;
                tri.cor = g_CorAtual;
                tri.pos = glm::vec2(cx, cy);
                tri.angulo = 0.0f;

                // Movimento bem mais lento e velocidade de rotação constante
                tri.vel = glm::vec2(gerarFloatAleatorio(-0.0001f, 0.0001f), gerarFloatAleatorio(-0.001f, 0.001f));
                tri.velRotacao = 0.05f; // Velocidade angular reduzida e fixa

                for (int i = 0; i < 3; ++i) {
                    tri.verticesLocais.push_back(g_PontosPendentes[i * 3] - cx);
                    tri.verticesLocais.push_back(g_PontosPendentes[i * 3 + 1] - cy);
                    tri.verticesLocais.push_back(0.0f);
                }

                g_Triangulos.push_back(tri);

                g_PontosPendentes.clear();
                glBindBuffer(GL_ARRAY_BUFFER, g_VBOPendentes);
                glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
            }
        }
    }

    void run() {
        srand((unsigned int)time(NULL));
        g_PontosPendentes.clear();
        g_Triangulos.clear();
        g_CorAtual = { 1.0f, 0.0f, 0.0f };

        Window win(800, 600, "Lista 3 - Triângulos Animados com Clique");
        GLFWwindow* window = win.getWindowId();

        glfwSetMouseButtonCallback(window, mouseButtonCallback);

        std::string vert_sdr = "#version 330 core\n"
            "layout (location = 0) in vec3 position;\n"
            "uniform mat4 transform;\n"
            "void main(){ gl_Position = transform * vec4(position, 1.0); }";

        std::string frag_sdr = "#version 330 core\n"
            "uniform vec4 tint;\n"
            "out vec4 result;\n"
            "void main(){ result = tint; }";

        Shader shader(vert_sdr, frag_sdr);
        GLint transformLoc = glGetUniformLocation(shader.getId(), "transform");

        // VAO/VBO para pontos pendentes
        glGenVertexArrays(1, &g_VAOPendentes);
        glBindVertexArray(g_VAOPendentes);
        glGenBuffers(1, &g_VBOPendentes);
        glBindBuffer(GL_ARRAY_BUFFER, g_VBOPendentes);
        glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // VAO/VBO reutilizável para desenhar os triângulos
        glGenVertexArrays(1, &g_VAOTriangulo);
        glBindVertexArray(g_VAOTriangulo);
        glGenBuffers(1, &g_VBOTriangulo);
        glBindBuffer(GL_ARRAY_BUFFER, g_VBOTriangulo);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glPointSize(8.0f);

        while (!glfwWindowShouldClose(window)) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.bind();

            // 1. Desenha e atualiza cada triângulo animado
            for (auto& tri : g_Triangulos) {
                // Atualiza posição e rotação
                tri.pos += tri.vel;
                tri.angulo += tri.velRotacao;

                // Rebate nas bordas da tela (NDC -1.0 a 1.0)
                if (tri.pos.x > 0.9f || tri.pos.x < -0.9f) tri.vel.x *= -1.0f;
                if (tri.pos.y > 0.9f || tri.pos.y < -0.9f) tri.vel.y *= -1.0f;

                // Monta a matriz de transformação: Matriz = Translação * Rotação
                glm::mat4 transform = glm::mat4(1.0f);
                transform = glm::translate(transform, glm::vec3(tri.pos.x, tri.pos.y, 0.0f));
                transform = glm::rotate(transform, glm::radians(tri.angulo), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                shader.bindUniform4f("tint", tri.cor.r, tri.cor.g, tri.cor.b, 1.0f);

                // Carrega os vértices locais do triângulo e desenha
                glBindVertexArray(g_VAOTriangulo);
                glBindBuffer(GL_ARRAY_BUFFER, g_VBOTriangulo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, tri.verticesLocais.size() * sizeof(float), tri.verticesLocais.data());
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }

            // 2. Desenha os pontos pendentes do próximo triângulo (sem animação/transformação)
            if (!g_PontosPendentes.empty()) {
                glm::mat4 identity = glm::mat4(1.0f);
                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(identity));
                shader.bindUniform4f("tint", g_CorAtual.r, g_CorAtual.g, g_CorAtual.b, 1.0f);

                glBindVertexArray(g_VAOPendentes);
                glDrawArrays(GL_POINTS, 0, (GLsizei)(g_PontosPendentes.size() / 3));
            }

            shader.unbind();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &g_VAOPendentes);
        glDeleteBuffers(1, &g_VBOPendentes);
        glDeleteVertexArrays(1, &g_VAOTriangulo);
        glDeleteBuffers(1, &g_VBOTriangulo);
    }

} // namespace Lista3