#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL/glew.h>

class Shader {
private:
    GLuint shaderProgramId; // Campo/membro que armazena o ID do programa

public:
    // Construtor: compila e linka os shaders
    Shader(const std::string& vertexSource, const std::string& fragmentSource);

    // Destrutor: limpa o programa da GPU
    ~Shader();

    // Ativa/Desativa o shader para uso
    void bind() const;
    void unbind() const;
	void bindUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;

    // Getter para expor o ID caso precise usar em funções nativas do OpenGL
    GLuint getId() const { return shaderProgramId; }

private:
    // Utilitário interno para verificar erros de compilação/linkagem
    void checkCompileErrors(GLuint shader, const std::string& type);
};

#endif // SHADER_H