#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h> /* include GLEW and new version of GL on Windows */
#include <GLFW/glfw3.h> /* GLFW helper library */

#include <glm/glm.hpp>
#include <math.h>




/*
 * Exercicio: desenhar 2 triangulos na tela com 4 modos:
 *   a) apenas preenchido      -> tecla 1
 *   b) apenas contorno        -> tecla 2
 *   c) apenas como pontos     -> tecla 3
 *   d) as 3 formas juntas     -> tecla 4
 */

int main() {
    // Initialise GLFW
    glewExperimental = true; // Needed for core profile
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "Tutorial 01", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    glewExperimental = true; // Needed in core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

	//Small script to ensure the end of the program after key ESC is pressed
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


    static const GLfloat triangles[] = {
        //triangle 0
       -0.5f, 0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       0.0f,  0.0f, 0.0f,
       //triangle 1
       0.5f, 0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.0f, 0.0f,
    };

    //static const GLfloat triangle1[] = {
    //   0.5f, 0.5f, 0.0f,
    //   0.5f, -0.5f, 0.0f,
    //   0.0f,  0.0f, 0.0f,
    //};

	// This is our Vertex Array Object, which will store the state associated with our vertex buffer (VAO)
    GLuint VAOTrianglesId;
    glGenVertexArrays(1, &VAOTrianglesId);
    glBindVertexArray(VAOTrianglesId);

    // This will identify our vertex buffer (VBO)
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);


    //Lines
    std::vector<GLfloat> lines = 
    {
	   -0.5, 0.5, 0.0,
       0.5, 0.5, 0.0,
       0.5, 0.5, 0.0,
	   0.5, -0.5, 0.0,
    };

	GLuint VAOLinesId;
    glGenVertexArrays(1, &VAOLinesId);
    glBindVertexArray(VAOLinesId);

    glVertexAttribPointer(
        0,          // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,          // size
        GL_FLOAT,   // type
        GL_FALSE,   // normalized?
        0,          // stride
        (void*)0    // array buffer offset
    );

	GLuint linebuffer;
	glGenBuffers(1, &linebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, linebuffer);
	glBufferData(GL_ARRAY_BUFFER, lines.size()*sizeof(GLfloat), lines.data(), GL_STATIC_DRAW);

    do {
        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindVertexArray(VAOLinesId);
       
        //Draw the triangle !
        //glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDrawArrays(GL_LINES, 0, lines.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);



    glfwTerminate();
    return 0;
}