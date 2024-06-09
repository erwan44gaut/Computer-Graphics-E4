#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common/GLShader.h"

struct Application
{
public:
    GLFWwindow *window;
    GLShader *shader;
    GLuint VBO, VAO, IBO;

    struct Vector3
    {
        float x, y, z;
    };

    struct Vertex
    {
        Vector3 position;
        Vector3 color;
    };

    Vertex vertices[3] = 
    {
        // Vertex 1
        { { -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f } }, // Red
        
        // Vertex 2
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f } }, // Green
        
        // Vertex 3
        { { 0.0f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f } } // Blue
    };

    unsigned int indices[6] = { 
        0, 1, 2,
        1, 2, 3
    };

    void Init()
    {
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window)
        {
            std::cerr << "Failed to create window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        // Initialize GLEW
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Create and load shaders
        shader = new GLShader();
        if (!shader->LoadVertexShader("vert.glsl"))
        {
            std::cerr << "Failed to load vertex shader" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!shader->LoadFragmentShader("frag.glsl"))
        {
            std::cerr << "Failed to load fragment shader" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!shader->Create())
        {
            std::cerr << "Failed to create shader program" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    void InitBuffers()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);

        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        int loc_position = glGetAttribLocation(shader->GetProgram(), "a_position");
        int loc_color = glGetAttribLocation(shader->GetProgram(), "a_color");
        
        glEnableVertexAttribArray(loc_position);
        glVertexAttribPointer(loc_position, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(loc_color);
        glVertexAttribPointer(loc_color, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        
        // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Render()
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Use our shader program
        glUseProgram(shader->GetProgram());
        
        // Bind the VAO
        glBindVertexArray(VAO);

        // Draw the triangle
        int N = sizeof(indices) / sizeof(unsigned int);
        glDrawElements(GL_TRIANGLES, N, GL_UNSIGNED_INT, 0);

        // Unbind the VAO
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    void Terminate()
    {
        // Deallocate resources
        shader->Destroy();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &IBO);
        glfwTerminate();
    }
};

int main(void)
{
    Application app;
    app.Init();
    app.InitBuffers();

    while (!glfwWindowShouldClose(app.window))
    {
        app.Render();
    }

    app.Terminate();

    return 0;
}