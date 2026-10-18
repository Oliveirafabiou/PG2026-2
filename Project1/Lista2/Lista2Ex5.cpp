#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

const float PI = 3.14159265358979323846f;

// COMPILACAO DOS SHADERS
GLuint shaderProgram(std::string const& vs, std::string const& fs) {
    char logStr[1024] = "";
    GLuint vid = glCreateShader(GL_VERTEX_SHADER);
    char const* vs_ptr = vs.c_str();
    glShaderSource(vid, 1, &vs_ptr, NULL);
    glCompileShader(vid);

    GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);
    char const* fs_ptr = fs.c_str();
    glShaderSource(fid, 1, &fs_ptr, NULL);
    glCompileShader(fid);

    GLuint program = glCreateProgram();
    glAttachShader(program, vid);
    glAttachShader(program, fid);
    glLinkProgram(program);

    glGetProgramInfoLog(program, 1023, NULL, logStr);
    std::cout << logStr << std::endl;

    glDetachShader(program, vid);
    glDetachShader(program, fid);
    glDeleteShader(vid);
    glDeleteShader(fid);

    return program;
}

// ==========================================
// FUNCOES GERADORAS DE GEOMETRIA (PARAMETRICAS)
// ==========================================

// 2. Circulo Preenchido (ou contorno se GL_LINE)
std::vector<float> gerarCirculo(float cx, float cy, float raio, int numSegmentos) {
    std::vector<float> vertices;
    // Centro do circulo
    vertices.push_back(cx);
    vertices.push_back(cy);
    vertices.push_back(0.0f);

    for (int i = 0; i <= numSegmentos; ++i) {
        float angulo = 2.0f * PI * i / numSegmentos;
        float x = cx + raio * cos(angulo);
        float y = cy + raio * sin(angulo);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(0.0f);
    }
    return vertices;
}

// ==========================================
// FUNCAO PRINCIPAL
// ==========================================
int main(void) {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Geometrias Parametricas", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) return -1;

    std::string vert_sdr = "#version 330 core\nlayout (location = 0) in vec3 position;\nvoid main(){ gl_Position = vec4(position, 1.0); }";
    std::string frag_sdr = "#version 330 core\nuniform vec4 tint;\nout vec4 result;\nvoid main(){ result = tint; }";

    GLuint programID = shaderProgram(vert_sdr, frag_sdr);
    glUseProgram(programID);
    GLint colorLoc = glGetUniformLocation(programID, "tint");

    // ==========================================
    // PROFE, É SÓ ESCOLHER UMA DAS GEOMETRIAS ABAIXO PARA DESENHAR, DESCOMENTANDO-A E COMENTANDO AS OUTRAS!!!
    // ==========================================

     //Circulo base:
     auto dados = gerarCirculo(0.0f, 0.0f, 0.5f, 50); GLenum modoDesenho = GL_TRIANGLE_FAN;


    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, dados.size() * sizeof(float), dados.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glLineWidth(3.0f);

    int viewPorts[4][4] = {
        {0, 0, 400, 300},
        {0, 300, 400, 300},
        {400, 0, 400, 300},
        {400, 300, 400, 300},
    };

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);

        for (int i = 0; i < 4; i++) {
            glViewport(viewPorts[i][0], viewPorts[i][1], viewPorts[i][2], viewPorts[i][3]);
            glBindVertexArray(VAO);

            // Se for a espiral, é usado GL_LINE_STRIP; para o restante, é usado TRIANGLE_FAN ou TRIANGLES
            if (modoDesenho == GL_LINE_STRIP) {
                glDrawArrays(GL_LINE_STRIP, 0, dados.size() / 3);
            }
            else if (modoDesenho == GL_TRIANGLE_FAN) {
                glDrawArrays(GL_TRIANGLE_FAN, 0, dados.size() / 3);
            }
            else {
                glDrawArrays(GL_TRIANGLES, 0, dados.size() / 3);
            }
        };

        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programID);
    glfwTerminate();
    return 0;
}