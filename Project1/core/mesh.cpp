#include "Mesh.h"
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Mesh::Mesh(const std::vector<float>& vertices) : vao_id(0), vbo_id(0) {

	// Cria e vincula o VAO
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	// Cria e vincula o VBO
	glGenBuffers(1, &vbo_id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	// Define os atributos do vértice (posição)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Desvincula o VAO e VBO para evitar alterações acidentais
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::~Mesh() {
	// Limpa o VAO e VBO da GPU
	glDeleteVertexArrays(1, &vao_id);
	glDeleteBuffers(1, &vbo_id);
}

void Mesh::draw() const {
	draw(GL_TRIANGLES);
}

void Mesh::draw(GLenum mode) const {
	glBindVertexArray(vao_id);
	glDrawArrays(mode, 0, 3);
	glBindVertexArray(0);
}