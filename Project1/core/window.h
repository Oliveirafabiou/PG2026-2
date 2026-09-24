#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <cassert>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>

class Window {
private:
	GLFWwindow* windowId; // Campo que armazena o ID da janela GLFW

public:
	// Construtor: Cria e abre uma janela GLFW com as dimensões e título fornecidos
    Window(int width, int height, std::string title);

	// Destrutor: fecha a janela e encerra o GLFW
    ~Window();

	// Getter para expor o ID da janela
    GLFWwindow* getWindowId() const { return windowId; }

};

#endif // WINDOW_H