#include "Lista2.h"
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Lista2 {

// Helper interno para compilação simples de shaders
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

    // Retorna para o submenu da Lista 2 sem abrir a janela do GLFW
    if (quadrante == 5) {
        return;
    }

    if (quadrante < 1 || quadrante > 4) {
        std::cout << "Opção inválida! Retornando ao menu...\n";
        return;
    }

    if (!glfwInit()) return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lista 2 - Quadrante Único", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return;

    GLuint programID = compileShaderHelper();
    GLint colorLoc = glGetUniformLocation(programID, "tint");

    // Triângulo simples de teste
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Limpa a tela inteira com fundo escuro
        glViewport(0, 0, 800, 600);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Define a Viewport do quadrante selecionado (Largura 400, Altura 300)
        int vx = 0, vy = 0;
        switch (quadrante) {
        case 1: vx = 400; vy = 300; break; // Q1: Superior Direito
        case 2: vx = 0;   vy = 300; break; // Q2: Superior Esquerdo
        case 3: vx = 0;   vy = 0;   break; // Q3: Inferior Esquerdo
        case 4: vx = 400; vy = 0;   break; // Q4: Inferior Direito
        }

        glViewport(vx, vy, 400, 300);

        // Desenha o conteúdo dentro do quadrante
        glUseProgram(programID);
        glUniform4f(colorLoc, 0.2f, 0.8f, 0.4f, 1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programID);
    glfwTerminate();
}

void renderizarTodosQuadrantes() {
    if (!glfwInit()) return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Lista 2 - Todos os Quadrantes", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return;

    GLuint programID = compileShaderHelper();
    GLint colorLoc = glGetUniformLocation(programID, "tint");

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Cores para cada quadrante
    float cores[4][4] = {
        {1.0f, 0.2f, 0.2f, 1.0f}, // Vermelho (Q1)
        {0.2f, 1.0f, 0.2f, 1.0f}, // Verde (Q2)
        {0.2f, 0.4f, 1.0f, 1.0f}, // Azul (Q3)
        {1.0f, 1.0f, 0.2f, 1.0f}  // Amarelo (Q4)
    };

    // Coordenadas das viewports: {x, y, largura, altura}
    int viewports[4][4] = {
        {400, 300, 400, 300}, // Q1
        {0,   300, 400, 300}, // Q2
        {0,   0,   400, 300}, // Q3
        {400, 0,   400, 300}  // Q4
    };

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glViewport(0, 0, 800, 600);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);
        glBindVertexArray(VAO);

        // Renderiza a geometria em cada uma das 4 viewports com uma cor diferente
        for (int i = 0; i < 4; ++i) {
            glViewport(viewports[i][0], viewports[i][1], viewports[i][2], viewports[i][3]);
            glUniform4f(colorLoc, cores[i][0], cores[i][1], cores[i][2], cores[i][3]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programID);
    glfwTerminate();
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

        if (option == 1) {
            renderizarQuadranteUnico();
        } else if (option == 2) {
            renderizarTodosQuadrantes();
        } else if (option == 3) {
            break;
        } else {
            std::cout << "Opção inválida! Tente novamente.\n";
        }
    }
}

} // namespace Lista2