#include "mesh.h"
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Mesh::Mesh(const std::vector<float>& vertices) : vao_id(0), vbo_id(0), vertexCount(0) {
    vertexCount = static_cast<int>(vertices.size() / 3);

    // Cria e vincula o VAO
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // Cria e vincula o VBO
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Define os atributos de posição (layout 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Desvincula para evitar alterações acidentais
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (vbo_id != 0) glDeleteBuffers(1, &vbo_id);
    if (vao_id != 0) glDeleteVertexArrays(1, &vao_id);
}

void Mesh::bind() const {
    glBindVertexArray(vao_id);
}

void Mesh::draw() const {
    draw(GL_TRIANGLES);
}

void Mesh::draw(GLenum mode) const {
    glBindVertexArray(vao_id);
    glDrawArrays(mode, 0, vertexCount);
    glBindVertexArray(0);
}

void Mesh::updateData(const std::vector<float>& vertices) {
    vertexCount = static_cast<int>(vertices.size() / 3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}