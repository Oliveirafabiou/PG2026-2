#ifndef MESH_H
#define MESH_H

#include <string>
#include <GL/glew.h>
#include <vector>

class Mesh {
private:
	GLuint vao_id; // Campo que armazena o ID do VAO (Vertex Array Object)
	GLuint vbo_id; // Campo que armazena o ID do VBO (Vertex Buffer Object)

public:
	// Construtor: cria e inicializa o VAO e VBO com os vértices fornecidos
    Mesh(const std::vector<float>& vertices);

    // Destrutor: limpa o VAO e VBO da GPU
    ~Mesh();

	// Desenha o VAO na tela
    void draw() const;
	void draw(GLenum mode) const;

private:

    // Ativa o VAO para uso
    void bind() const;


};

#endif // MESH_H