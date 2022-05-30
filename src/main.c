#include "main.h"

int main(int argc,char ** argv)
{
    GLFWwindow * window;
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    };

    if(!glfwInit())
    {
        return 1;
    }

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    if(!window)
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}