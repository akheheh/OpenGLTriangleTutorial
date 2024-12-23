#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLEnv/GLEnv.h>
/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/

struct VIEW
{
    int WIDTH;
    int HEIGHT;
    float *clearColor;
};

// Vertex shader
const char *vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 vertex;
    layout (location = 1) in vec3 vertexColor;
    out vec4 fragmentColor;
    void main() {
        gl_Position = vec4(vertex, 1.0f);
        fragmentColor = vec4(vertexColor, 1.0f);
    }
)";

// Fragment shader
const char *fragmentShaderSource = R"(
    #version 330 core
    in vec4 fragmentColor;
    out vec4 pixelColor;
    uniform float time;
    void main() {
        pixelColor = vec4(fragmentColor.r, fragmentColor.g + sin(time), fragmentColor.b, 1.0f);
    }
)";

float clearColorArray[] = {0.0f, 0.0f, 0.0f, 1.0f};
auto GLView = VIEW{1920, 1080, clearColorArray};

auto [WIDTH, HEIGHT, CLEAR_COLOR] = GLView;

int main()
{
    GLFWwindow *window = createOpenGLEnv(1920, 1080, "GL");

    // vertex data
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f};
    
    float triangle1[] = {
        -0.75f, 0.0f, 0.0f,
        -0.5f, 0.7f, 0.0f,
        -0.25f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    float triangle2[] = {
        0.25f, 0.0f, 0.0f,
        0.5f, 0.75f, 0.0f,
        0.75f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f,
        0.25f, 0.0f, 0.25f,
        1.0f, 0.0f, 0.0f
    };

    // Create and bind the Vertex Array
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    //Create 2 different VAOs for each triangle
    GLuint VAOs[2];
    glGenVertexArrays(2, VAOs);
    glBindVertexArray(VAOs[0]);

    GLuint VBOs[2];

    glGenBuffers(2, VBOs);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1), triangle1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) (9 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    

    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2), triangle2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) (9 * sizeof(float)));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    /*
    glBindVertexArray(VAO);

    // Create and bind the Vertex Buffer
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Load vertext data into the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Tell the Vertex Array how to read data for the given attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(9 * sizeof(float)));

    // Turn on the attribute
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    */
    // Create a vertex shader, fragment shader, and a shader program
    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    shaderProgram = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glViewport(0, 0, WIDTH, HEIGHT);

    /*
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    vec = trans * vec;
    std::cout << vec.x << vec.y << vec.z << std::endl;
    */

   int timeLoc = glGetUniformLocation(shaderProgram, "time");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        GLGeneralSetup(window, CLEAR_COLOR);

        // Draw the triangle
        glUseProgram(shaderProgram);
        float timeValue = glfwGetTime();
        glUniform1f(timeLoc, timeValue);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}