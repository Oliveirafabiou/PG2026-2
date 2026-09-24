#include <vector>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const float PI = 3.14159265358979323846f;

std::vector<float> gerarTriangulo(float cx, float cy, float tamanho) {
    std::vector<float> vertices;
    vertices.push_back(cx); vertices.push_back(cy + tamanho / 2.0f); vertices.push_back(0.0f); // Topo
    vertices.push_back(cx - tamanho / 2.0f); vertices.push_back(cy - tamanho / 2.0f); vertices.push_back(0.0f); // Esquerda
    vertices.push_back(cx + tamanho / 2.0f); vertices.push_back(cy - tamanho / 2.0f); vertices.push_back(0.0f); // Direita
    return vertices;
}

// Circulo 
std::vector<float> gerarCirculo(float cx, float cy, float raio, int numSegmentos) {
    std::vector<float> vertices;
    // Centro do circulo
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

// Octogono
std::vector<float> gerarPoligono(float cx, float cy, float raio, int lados) {
    std::vector<float> vertices;
    for (int i = 0; i < lados; ++i) {
        float angulo1 = 2.0f * PI * i / lados;
        float angulo2 = 2.0f * PI * (i + 1) / lados;

        // Centro
        vertices.push_back(cx); vertices.push_back(cy); vertices.push_back(0.0f);
        // Ponto atual
        vertices.push_back(cx + raio * cos(angulo1)); vertices.push_back(cy + raio * sin(angulo1)); vertices.push_back(0.0f);
        // Proximo ponto
        vertices.push_back(cx + raio * cos(angulo2)); vertices.push_back(cy + raio * sin(angulo2)); vertices.push_back(0.0f);
    }
    return vertices;
}

// Pac-Man 
std::vector<float> gerarPacMan(float cx, float cy, float raio, int numSegmentos) {
    std::vector<float> vertices;
    // Centro
    vertices.push_back(cx);
    vertices.push_back(cy);
    vertices.push_back(0.0f);

    // Deixa uma abertura na boca
    float anguloInicio = PI / 6.0f;     // 30 graus
    float anguloFim = 2.0f * PI - (PI / 6.0f); // 330 graus

    int segmentosBoca = numSegmentos * 10 / 12; // Proporcao da abertura

    for (int i = 0; i <= segmentosBoca; ++i) {
        float angulo = anguloInicio + (anguloFim - anguloInicio) * i / segmentosBoca;
        vertices.push_back(cx + raio * cos(angulo));
        vertices.push_back(cy + raio * sin(angulo));
        vertices.push_back(0.0f);
    }
    return vertices;
}

// Fatia de Pizza
std::vector<float> gerarFatiaPizza(float cx, float cy, float raio, float anguloInicio, float anguloFim, int numSegmentos) {
    std::vector<float> vertices;
    // Centro
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

// Estrela 
std::vector<float> gerarEstrela(float cx, float cy, float raioExterno, float raioInterno) {
    std::vector<float> vertices;
    int pontas = 5;
    int totalVertices = pontas * 2;

    std::vector<float> pontosX(totalVertices);
    std::vector<float> pontosY(totalVertices);

    for (int i = 0; i < totalVertices; ++i) {
        float raio = (i % 2 == 0) ? raioExterno : raioInterno;
        float angulo = i * (2.0f * PI / totalVertices) - (PI / 2.0f); // Alinhada para cima
        pontosX[i] = cx + raio * cos(angulo);
        pontosY[i] = cy + raio * sin(angulo);
    }

    // Monta os triangulos da estrela conectados ao centro
    for (int i = 0; i < totalVertices; ++i) {
        int proximo = (i + 1) % totalVertices;
        vertices.push_back(cx); vertices.push_back(cy); vertices.push_back(0.0f);
        vertices.push_back(pontosX[i]); vertices.push_back(pontosY[i]); vertices.push_back(0.0f);
        vertices.push_back(pontosX[proximo]); vertices.push_back(pontosY[proximo]); vertices.push_back(0.0f);
    }
    return vertices;
}

// Espiral 
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

