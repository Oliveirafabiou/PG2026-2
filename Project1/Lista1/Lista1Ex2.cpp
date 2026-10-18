#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Lista1.h"

namespace Lista1 {

    const float PI = 3.14159265358979323846f;

    static GLuint shaderProgram(std::string const& vs, std::string const& fs) {
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

    // ==========================================
    // FUNÇÕES GERADORAS DE GEOMETRIA
    // ==========================================

    std::vector<float> gerarCirculo(float cx, float cy, float raio, int numSegmentos) {
        std::vector<float> vertices;
        vertices.push_back(cx);
        vertices.push_back(cy);
        vertices.push_back(0.0f);

        for (int i = 0; i <= numSegmentos; ++i) {
            float angulo = 2.0f * PI * i / numSegmentos;
            float x = cx + raio * cos(angulo);
            float y = cy + raio * sin(angulo);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
        return vertices;
    }

    std::vector<float> gerarPoligono(float cx, float cy, float raio, int lados) {
        std::vector<float> vertices;
        for (int i = 0; i < lados; ++i) {
            float angulo1 = 2.0f * PI * i / lados;
            float angulo2 = 2.0f * PI * (i + 1) / lados;

            vertices.push_back(cx); vertices.push_back(cy); vertices.push_back(0.0f);
            vertices.push_back(cx + raio * cos(angulo1)); vertices.push_back(cy + raio * sin(angulo1)); vertices.push_back(0.0f);
            vertices.push_back(cx + raio * cos(angulo2)); vertices.push_back(cy + raio * sin(angulo2)); vertices.push_back(0.0f);
        }
        return vertices;
    }

    std::vector<float> gerarPacMan(float cx, float cy, float raio, int numSegmentos) {
        std::vector<float> vertices;
        vertices.push_back(cx);
        vertices.push_back(cy);
        vertices.push_back(0.0f);

        float anguloInicio = PI / 6.0f;
        float anguloFim = 2.0f * PI - (PI / 6.0f);

        int segmentosBoca = numSegmentos * 10 / 12;

        for (int i = 0; i <= segmentosBoca; ++i) {
            float angulo = anguloInicio + (anguloFim - anguloInicio) * i / segmentosBoca;
            vertices.push_back(cx + raio * cos(angulo));
            vertices.push_back(cy + raio * sin(angulo));
            vertices.push_back(0.0f);
        }
        return vertices;
    }

    std::vector<float> gerarFatiaPizza(float cx, float cy, float raio, float anguloInicio, float anguloFim, int numSegmentos) {
        std::vector<float> vertices;
        vertices.push_back(cx);
        vertices.push_back(cy);
        vertices.push_back(0.0f);

        for (int i = 0; i <= numSegmentos; ++i) {
            float angulo = anguloInicio + (anguloFim - anguloInicio) * i / numSegmentos;
            vertices.push_back(cx + raio * cos(angulo));
            vertices.push_back(cy + raio * sin(angulo));
            vertices.push_back(0.0f);
        }
        return vertices;
    }

    std::vector<float> gerarEstrela(float cx, float cy, float raioExterno, float raioInterno) {
        std::vector<float> vertices;
        int pontas = 5;
        int totalVertices = pontas * 2;

        std::vector<float> pontosX(totalVertices);
        std::vector<float> pontosY(totalVertices);

        for (int i = 0; i < totalVertices; ++i) {
            float raio = (i % 2 == 0) ? raioExterno : raioInterno;
            float angulo = i * (2.0f * PI / totalVertices) - (PI / 2.0f);
            pontosX[i] = cx + raio * cos(angulo);
            pontosY[i] = cy + raio * sin(angulo);
        }

        for (int i = 0; i < totalVertices; ++i) {
            int proximo = (i + 1) % totalVertices;
            vertices.push_back(cx); vertices.push_back(cy); vertices.push_back(0.0f);
            vertices.push_back(pontosX[i]); vertices.push_back(pontosY[i]); vertices.push_back(0.0f);
            vertices.push_back(pontosX[proximo]); vertices.push_back(pontosY[proximo]); vertices.push_back(0.0f);
        }
        return vertices;
    }

    std::vector<float> gerarEspiral(float cx, float cy, float voltas, int numSegmentos) {
        std::vector<float> vertices;
        float maxRaio = 0.8f;

        for (int i = 0; i < numSegmentos; ++i) {
            float t = (float)i / numSegmentos;
            float angulo = t * voltas * 2.0f * PI;
            float raio = t * maxRaio;

            float x = cx + raio * cos(angulo);
            float y = cy + raio * sin(angulo);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }
        return vertices;
    }

    // Helper para recalcular a forma selecionada e atualizar a GPU
    static void carregarForma(int indice, std::vector<float>& dados, GLenum& modoDesenho, GLuint VBO) {
        switch (indice) {
        case 0:
            std::cout << "[1/7] Círculo\n";
            dados = gerarCirculo(0.0f, 0.0f, 0.5f, 50);
            modoDesenho = GL_TRIANGLE_FAN;
            break;
        case 1:
            std::cout << "[2/7] Octógono\n";
            dados = gerarPoligono(0.0f, 0.0f, 0.6f, 8);
            modoDesenho = GL_TRIANGLES;
            break;
        case 2:
            std::cout << "[3/7] Pentágono\n";
            dados = gerarPoligono(0.0f, 0.0f, 0.6f, 5);
            modoDesenho = GL_TRIANGLES;
            break;
        case 3:
            std::cout << "[4/7] Pac-Man\n";
            dados = gerarPacMan(0.0f, 0.0f, 0.6f, 50);
            modoDesenho = GL_TRIANGLE_FAN;
            break;
        case 4:
            std::cout << "[5/7] Fatia de Pizza\n";
            dados = gerarFatiaPizza(0.0f, 0.0f, 0.6f, 0.0f, PI / 3.0f, 30);
            modoDesenho = GL_TRIANGLE_FAN;
            break;
        case 5:
            std::cout << "[6/7] Estrela\n";
            dados = gerarEstrela(0.0f, 0.0f, 0.6f, 0.25f);
            modoDesenho = GL_TRIANGLES;
            break;
        case 6:
            std::cout << "[7/7] Espiral\n";
            dados = gerarEspiral(0.0f, 0.0f, 4.0f, 200);
            modoDesenho = GL_LINE_STRIP;
            break;
        }

        // Envia os novos vértices para a GPU
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, dados.size() * sizeof(float), dados.data(), GL_STATIC_DRAW);
    }

    // ==========================================
    // FUNÇÃO PRINCIPAL
    // ==========================================
    int runLista1Ex2(void) {
        if (!glfwInit()) return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(800, 600, "Geometrias Paramétricas (Use as setas para alternar)", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(window);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) return -1;

        std::string vert_sdr = "#version 330 core\nlayout (location = 0) in vec3 position;\nvoid main(){ gl_Position = vec4(position, 1.0); }";
        std::string frag_sdr = "#version 330 core\nuniform vec4 tint;\nout vec4 result;\nvoid main(){ result = tint; }";

        GLuint programID = shaderProgram(vert_sdr, frag_sdr);
        glUseProgram(programID);
        GLint colorLoc = glGetUniformLocation(programID, "tint");

        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glLineWidth(3.0f);

        int formaAtual = 0;
        const int totalFormas = 7;
        std::vector<float> dados;
        GLenum modoDesenho = GL_TRIANGLES;

        std::cout << "\n=============================================\n";
        std::cout << "Use as setas [ESQUERDA / DIREITA] no teclado para trocar de forma!\n";
        std::cout << "Pressione ESC para sair.\n";
        std::cout << "=============================================\n";

        // Carrega a primeira forma (Círculo)
        carregarForma(formaAtual, dados, modoDesenho, VBO);

        // Flags para controlar o clique das teclas (evita repetição instantânea ao segurar)
        bool teclaEsquerdaPressionada = false;
        bool teclaDireitaPressionada = false;

        while (!glfwWindowShouldClose(window)) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            // NAVEGAÇÃO POR SETAS COM TRATAMENTO DE DEBOUNCE
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                if (!teclaDireitaPressionada) {
                    formaAtual = (formaAtual + 1) % totalFormas;
                    carregarForma(formaAtual, dados, modoDesenho, VBO);
                    teclaDireitaPressionada = true;
                }
            }
            else {
                teclaDireitaPressionada = false;
            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                if (!teclaEsquerdaPressionada) {
                    formaAtual = (formaAtual - 1 + totalFormas) % totalFormas;
                    carregarForma(formaAtual, dados, modoDesenho, VBO);
                    teclaEsquerdaPressionada = true;
                }
            }
            else {
                teclaEsquerdaPressionada = false;
            }

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(programID);
            glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

            glBindVertexArray(VAO);

            if (modoDesenho == GL_LINE_STRIP) {
                glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)(dados.size() / 3));
            }
            else if (modoDesenho == GL_TRIANGLE_FAN) {
                glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)(dados.size() / 3));
            }
            else {
                glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(dados.size() / 3));
            }

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(programID);
        glfwTerminate();
        return 0;
    }
} // namespace Lista1