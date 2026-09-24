#include "Shader.h"
#include <iostream>
#include <vector>

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) : shaderProgramId(0) {

    // Compila Vertex Shader
    GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);
    const char* vs_ptr = vertexSource.c_str();
    glShaderSource(vertexId, 1, &vs_ptr, NULL);
    glCompileShader(vertexId);
    checkCompileErrors(vertexId, "VERTEX");

    // Compila Fragment Shader
    GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs_ptr = fragmentSource.c_str();
    glShaderSource(fragmentId, 1, &fs_ptr, NULL);
    glCompileShader(fragmentId);
    checkCompileErrors(fragmentId, "FRAGMENT");

    // Linka o Programa
    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexId);
    glAttachShader(shaderProgramId, fragmentId);
    glLinkProgram(shaderProgramId);
    checkCompileErrors(shaderProgramId, "PROGRAM");

    // Limpeza dos shaders intermediários
    glDetachShader(shaderProgramId, vertexId);
    glDetachShader(shaderProgramId, fragmentId);
    glDeleteShader(vertexId);
    glDeleteShader(fragmentId);
}

Shader::~Shader() {
    // Apaga o programa da memória da GPU quando o objeto for destruído
    if (shaderProgramId != 0) {
        glDeleteProgram(shaderProgramId);
    }
}

void Shader::bind() const {
    glUseProgram(shaderProgramId);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::bindUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    GLint location = glGetUniformLocation(shaderProgramId, name.c_str());
    if (location == -1) {
        std::cerr << "[ERROR::SHADER] Uniform '" << name << "' not found in shader program." << std::endl;
        return;
    }
	glUniform4f(location, v0, v1, v2, v3);
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    char logStr[1024] = { 0 };

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, sizeof(logStr), NULL, logStr);
            std::cerr << "[ERROR::SHADER_COMPILATION_ERROR] Type: " << type << "\n"
                << logStr << "\n---------------------------------------------------" << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, sizeof(logStr), NULL, logStr);
            std::cerr << "[ERROR::PROGRAM_LINKING_ERROR] Type: " << type << "\n"
                << logStr << "\n---------------------------------------------------" << std::endl;
        }
    }
}