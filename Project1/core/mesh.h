#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL/glew.h>

class Mesh {
private:
    GLuint vao_id;    // ID do VAO (Vertex Array Object)
    GLuint vbo_id;    // ID do VBO (Vertex Buffer Object)
    int vertexCount;  // Quantidade total de vértices na malha

public:
    // Construtor: cria e inicializa VAO e VBO com o vetor de vértices fornecido
    Mesh(const std::vector<float>& vertices);

    // Destrutor: libera VAO e VBO da memória da GPU
    ~Mesh();

    // Desenha os vértices usando o modo fornecido (padrão: GL_TRIANGLES)
    void draw() const;
    void draw(GLenum mode) const;

    // Atualiza os dados do VBO dinamicamente caso a geometria mude em tempo de execução
    void updateData(const std::vector<float>& vertices);

private:
    void bind() const;
};

#endif // MESH_H