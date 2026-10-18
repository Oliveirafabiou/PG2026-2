#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture {
private:
    GLuint tex_id;      // ID da textura gerado pelo OpenGL
    int width;          // Largura da imagem em pixels
    int height;         // Altura da imagem em pixels
    int nrChannels;     // Número de canais de cor (RGB, RGBA, etc.)

public:
    // Construtor padrão (cria um objeto vazio)
    Texture();

    // Construtor que já carrega o arquivo de imagem do caminho especificado
    Texture(const std::string& filePath);

    // Destrutor: deleta a textura da memória da GPU
    ~Texture();

    // Carrega uma nova imagem do disco
    bool load(const std::string& filePath);

    // Activa e vincula a textura em um slot/unidade específica (padrão é o slot 0)
    void bind(unsigned int slot = 0) const;

    // Desvincula qualquer textura do slot atual
    void unbind() const;

    // Getters para consultar as propriedades da textura
    GLuint getId() const { return tex_id; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getChannels() const { return nrChannels; }
};

#endif // TEXTURE_H