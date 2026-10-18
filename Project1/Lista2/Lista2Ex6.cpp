#include <iostream>
#include "../core/window.h"
#include "../core/shader.h"
#include "../core/mesh.h"
#include "../utils/geometryBuilder.cpp"

#include <stdlib.h>
#include <vector>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>

int main(void) {
	// Inicializa��o da janela
	Window window(800, 600, "Exerc�cio 6 - Gravatinha de Tri�ngulos");

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

	// Cria��o do shader
	Shader shader(vert_sdr, frag_sdr);

	// Cria��o da mesh com VAO e VBO
	Mesh mesh(gerarTriangulo(0.0f, 0.0f, 0.5f));

    // Ajusta o tamanho dos pontos para ficarem vis�veis junto com o desenho
    glPointSize(8.0f);
    // Ajusta a grossura das linhas de contorno 
    glLineWidth(2.0f);

    // Loop principal
    while (glfwGetKey(window.getWindowId(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.getWindowId()) == 0) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

		shader.bind();
		shader.bindUniform4f("tint", 0.0f, 1.0f, 0.0f, 1.0f);
        mesh.draw();
       
        glfwSwapBuffers(window.getWindowId());
        glfwPollEvents();
    }

    return 0;
}