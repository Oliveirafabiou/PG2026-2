#include "Lista2.h"
#include "../core/window.h"
#include "../core/shader.h"
#include "../core/mesh.h"

#include <iostream>
#include <vector>

namespace Lista2 {

    static const std::string vertShaderStr =
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
        "void main(){ gl_Position = vec4(position, 1.0); }";

    static const std::string fragShaderStr =
        "#version 330 core\n"
        "uniform vec4 tint;\n"
        "out vec4 result;\n"
        "void main(){ result = tint; }";

    void renderizarQuadranteUnico() {
        int quadrante = 1;
        std::cout << "\n--- Escolha o Quadrante ---\n";
        std::cout << "1. Superior Direito (Q1)\n";
        std::cout << "2. Superior Esquerdo (Q2)\n";
        std::cout << "3. Inferior Esquerdo (Q3)\n";
        std::cout << "4. Inferior Direito (Q4)\n";
        std::cout << "5. Voltar ao Menu Anterior\n";
        std::cout << "Opção: ";
        std::cin >> quadrante;

        if (quadrante == 5) return;
        if (quadrante < 1 || quadrante > 4) {
            std::cout << "Opção inválida!\n";
            return;
        }

        // Instancia a janela via Window do core
        Window win(800, 600, "Lista 2 - Quadrante Único");
        GLFWwindow* window = win.getWindowId();

        // Instancia o Shader via Shader do core
        Shader shader(vertShaderStr, fragShaderStr);

        // Triângulo de teste via Mesh do core
        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };
        Mesh mesh(vertices);

        while (!glfwWindowShouldClose(window)) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            glViewport(0, 0, 800, 600);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            int vx = 0, vy = 0;
            switch (quadrante) {
            case 1: vx = 400; vy = 300; break;
            case 2: vx = 0;   vy = 300; break;
            case 3: vx = 0;   vy = 0;   break;
            case 4: vx = 400; vy = 0;   break;
            }

            glViewport(vx, vy, 400, 300);

            shader.bind();
            shader.bindUniform4f("tint", 0.2f, 0.8f, 0.4f, 1.0f);
            mesh.draw(GL_TRIANGLES);
            shader.unbind();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void renderizarTodosQuadrantes() {
        Window win(800, 600, "Lista 2 - Todos os Quadrantes");
        GLFWwindow* window = win.getWindowId();

        Shader shader(vertShaderStr, fragShaderStr);

        std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };
        Mesh mesh(vertices);

        float cores[4][4] = {
            {1.0f, 0.2f, 0.2f, 1.0f},
            {0.2f, 1.0f, 0.2f, 1.0f},
            {0.2f, 0.4f, 1.0f, 1.0f},
            {1.0f, 1.0f, 0.2f, 1.0f}
        };

        int viewports[4][4] = {
            {400, 300, 400, 300},
            {0,   300, 400, 300},
            {0,   0,   400, 300},
            {400, 0,   400, 300}
        };

        while (!glfwWindowShouldClose(window)) {
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window, true);

            glViewport(0, 0, 800, 600);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.bind();
            for (int i = 0; i < 4; ++i) {
                glViewport(viewports[i][0], viewports[i][1], viewports[i][2], viewports[i][3]);
                shader.bindUniform4f("tint", cores[i][0], cores[i][1], cores[i][2], cores[i][3]);
                mesh.draw(GL_TRIANGLES);
            }
            shader.unbind();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void run() {
        int option;
        while (true) {
            std::cout << "\n--- Submenu Lista 2 ---\n";
            std::cout << "1. Escolher quadrante da tela para renderizar\n";
            std::cout << "2. Renderizar em todos os quadrantes\n";
            std::cout << "3. Voltar ao Menu Principal\n";
            std::cout << "Opção: ";
            std::cin >> option;

            if (option == 1) renderizarQuadranteUnico();
            else if (option == 2) renderizarTodosQuadrantes();
            else if (option == 3) break;
        }
    }

} // namespace Lista2