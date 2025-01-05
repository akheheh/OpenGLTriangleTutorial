#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <GLEnv/GLEnv.h>
#include <Shader/shader.h>
#include <STB/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
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

float clearColorArray[] = {0.0f, 0.0f, 0.0f, 1.0f};
auto GLView = VIEW{1920, 1080, clearColorArray};

auto [WIDTH, HEIGHT, CLEAR_COLOR] = GLView;

int main()
{
    GLFWwindow *window = createOpenGLEnv(WIDTH, HEIGHT, "GL");

    // Make a shader program
    Shader a_Shader("vertex_shader.glsl", "fragment_shader.glsl");
    // Call it once, resize callback will call it again with resize dimensions

    // Make a VAO and VBO
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex data: vertex coordinates, texture coordinates
    float vertexData[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);


    // Load our texture
    int width, height, colorChannels;
    // Flip image if texture coordinate origin is at the top does not match
    stbi_set_flip_vertically_on_load(true);
    unsigned char *textureData = stbi_load("./images/beach_min.jpg", &width, &height, &colorChannels, 0);

    // Make a GL texture object
    GLuint textureObj;
    glGenTextures(1, &textureObj);
    //Activate texture unit 0
    glActiveTexture(GL_TEXTURE0);

    // Bind to GL_TEXURE_2D, which is itself linked to the active texture unit
    glBindTexture(GL_TEXTURE_2D, textureObj);

    // load texture data to the texture object binding

    // If texture is RGBA aka 4 color channels, use GL_RGBA, otherwise use GL_RGB
    if (colorChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    }

    // Make a mipmap for the texture
    glGenerateMipmap(GL_TEXTURE_2D);

    // Texture filtering
    float borderColor[] = {0.2f, 0.77f, 1.0f, 1.0f};
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load another texture
    // First, a new texture object
    GLuint textureObj2;
    glGenTextures(1, &textureObj2);
    // Activate another texture unit, this time unit 1
    glActiveTexture(GL_TEXTURE1);
    // Bind the new active texture
    glBindTexture(GL_TEXTURE_2D, textureObj2);

    // Get the texture source data
    unsigned char *textureData2 = stbi_load("./images/tile.jpg", &width, &height, &colorChannels, 0);

    // Load texture data to the texture object binding
    if (colorChannels == 4)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData2);
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData2);
    }
    
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    // texture cleanup
    stbi_image_free(textureData);
    stbi_image_free(textureData2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    // Z-buffer to track what goes in front of what
    // OpenGL automatically makes a Z-buffer
    // We have to turn on depth testing for OpenGL to know what goes in front of what

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    // Clear the depth buffer
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT), done in the GLEnv function;
    a_Shader.use();
    int timeLoc = glGetUniformLocation(a_Shader.ID, "time");
    glUniform1i(glGetUniformLocation(a_Shader.ID, "texture1"), 0);
    // Likewise, glUniform1i(getLocationOfSecondSampler, set it to texture unit 1)
    glUniform1i(glGetUniformLocation(a_Shader.ID, "texture2"), 1);

    //Get mouse coordinates for mouse translation
    double mouseX, mouseY;
    double normalizedX, normalizedY;

    //Make the model, view and projection matrix for 3D 

    // Model matrix, rotate the object so it's like it's on a surface
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // View matrix, move it slightly back so it's in view
    // Equivalently, move the scene forwards
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // Projection matrix, use perspective projection
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(-45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    //Render loop
    while (!glfwWindowShouldClose(window))
    {
        //Set the mouse coordinates to 0 at initialization
        mouseX = 0;
        mouseY = 0;

        //General stuff
        processInput(window);
        GLGeneralSetup(window, CLEAR_COLOR);

        //get the mouse coordinates
        glfwGetCursorPos(window, &mouseX, &mouseY);

        //Convert the mouse coordinates to NDC for interaction
        normalizedX = -1.0 + 2.0 * mouseX / (double)WIDTH;
        normalizedY = 1.0 - 2.0 * mouseY / (double)HEIGHT;
        
        //Get the current time
        float time = glfwGetTime();

        //A transformation matrix for animating the object:
        //Matrix translation using mouse coordinates
        //Matrix rotation using time value, along the Z axis
        glm::mat4 transformationMatrix = glm::mat4(1.0f);
        transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-1.0f * normalizedX, -1.0f * normalizedY, 0.0));
        transformationMatrix = glm::rotate(transformationMatrix, time, glm::vec3(0, 0, 1));

        //Update the transform matrix uniform with the GLM matrix
        GLuint transformLoc = glGetUniformLocation(a_Shader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

        //Update the model, view and projection matrix uniforms with their GLM matrix
        GLuint modelLoc = glGetUniformLocation(a_Shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        GLuint viewLoc = glGetUniformLocation(a_Shader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projectionLoc = glGetUniformLocation(a_Shader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Updating time uniform for texture animation
        glUniform1f(timeLoc, time);
        // Activate and bind texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureObj);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureObj2);
        
        //Bind the VAO
        glBindVertexArray(VAO);

        //Draw the object
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //General stuff
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}