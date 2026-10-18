#include "Texture.h"
#include <iostream>

// Define a implementação do stb_image apenas aqui no .cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() : tex_id(0), width(0), height(0), nrChannels(0) {
}

Texture::Texture(const std::string& filePath) : tex_id(0), width(0), height(0), nrChannels(0) {
    load(filePath);
}

Texture::~Texture() {
    if (tex_id != 0) {
        glDeleteTextures(1, &tex_id);
    }
}

bool Texture::load(const std::string& filePath) {
    // Se já houver uma textura carregada neste objeto, limpa antes de carregar a nova
    if (tex_id != 0) {
        glDeleteTextures(1, &tex_id);
        tex_id = 0;
    }

    // Inverte a imagem no eixo Y (OpenGL tem a origem [0,0] no canto inferior esquerdo)
    stbi_set_flip_vertically_on_load(true);

    // Carrega os dados da imagem a partir do arquivo
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        std::cerr << "[ERROR::TEXTURE] Failed to load image from path: " << filePath << std::endl;
        return false;
    }

    // Identifica o formato da imagem com base no número de canais
    GLenum format = GL_RGB;
    if (nrChannels == 1) {
        format = GL_RED;
    }
    else if (nrChannels == 3) {
        format = GL_RGB;
    }
    else if (nrChannels == 4) {
        format = GL_RGBA;
    }

    // Gera o ID da textura no OpenGL
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    // Configura os parâmetros de repetição (Wrap)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Configura os parâmetros de filtragem (Minification / Magnification)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Envia os dados dos pixels para a GPU
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // Gera os níveis de Mipmap automaticamente
    glGenerateMipmap(GL_TEXTURE_2D);

    // Libera a memória da CPU, pois os dados já estão na GPU
    stbi_image_free(data);

    // Desvincula a textura por segurança
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, tex_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}