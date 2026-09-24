#include "window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Window::Window(int width, int height, std::string title) {

    // Inicialização do GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

	//Versão do OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Cria a janela GLFW
    windowId = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!windowId) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	//Seta a janela atual como contexto OpenGL
    glfwMakeContextCurrent(windowId);

    // Inicialização da GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetInputMode(windowId, GLFW_STICKY_KEYS, GL_TRUE);
}

Window::~Window() {
    // Fecha a janela e encerra o GLFW
    glfwDestroyWindow(windowId);
    glfwTerminate();
}

