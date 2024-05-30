#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "pixeldye.h"

#include <iostream>
#include "../Vector4.h"
#include "../Matrix.h"
#include "../model.h"
#include <stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);

//camera
Vector4 cameraPos(0.0f, 0.0f, 3.0f, 1.0f);
Vector4 cameraTarget(0.0f, 0.0f, 0.0f, 1.0f);
Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);

bool startPress = true;
float anglex = 0.0f;
float angley = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float nowX = 800.0f / 2.0;
float nowY = 600.0 / 2.0;
float fov = 45.0f;
Vector4 axle(0.0f, 0.0f, 0.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

   // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Model ourModel("./cube/cube.obj");

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    unsigned char* data = new unsigned char[3 * SCR_WIDTH * SCR_HEIGHT * sizeof(unsigned char)];
    double* zbuffer = new double[SCR_WIDTH * SCR_HEIGHT * sizeof(double)];

    Matrix preModel = Matrix::Scale(1.0f, 1.0f, 1.0f);
    Matrix view = Matrix::View(cameraPos, cameraTarget, up);
    Matrix proj = Matrix::Persp(fov, 0.1f, 100.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT);

    float a = (SCR_WIDTH) / 2.0;
    float b = (SCR_HEIGHT) / 2.0;

    Matrix scale = Matrix::Scale(a, b, 1.0f);
    Matrix trans = Matrix::Translate(a, b, 0.0f);

    int width, height, nrChannels;
    unsigned char* imagedata = stbi_load("./cube/default.png", &width, &height, &nrChannels, 0);


    //render
    //render end


    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----

        processInput(window);

        //render

        for (unsigned int i = 0; i < SCR_WIDTH * SCR_HEIGHT; i++)
        {
            data[i * 3] = (unsigned char)(0.0f);
            data[i * 3 + 1] = (unsigned char)(255.0f);
            data[i * 3 + 2] = (unsigned char)(0.0f);
            zbuffer[i] = 1.0;
        }

        int size = ourModel.meshes[0].indices.size();
        for (int i = 0; i < size / 3; i++) {

            int index = i * 3;
            Vector4 vertices[] = {
             Vector4(ourModel.meshes[0].vertices[index].Position.x, ourModel.meshes[0].vertices[index].Position.y, ourModel.meshes[0].vertices[index].Position.z, 1.0f),
             Vector4(ourModel.meshes[0].vertices[index + 1].Position.x, ourModel.meshes[0].vertices[index + 1].Position.y, ourModel.meshes[0].vertices[index + 1].Position.z, 1.0f),
             Vector4(ourModel.meshes[0].vertices[index + 2].Position.x, ourModel.meshes[0].vertices[index + 2].Position.y, ourModel.meshes[0].vertices[index + 2].Position.z, 1.0f)
            };

            Vector4 verticesuv[] = {
              Vector4(ourModel.meshes[0].vertices[index].TexCoords.x, ourModel.meshes[0].vertices[index].TexCoords.y, 0.0f, 1.0f),
              Vector4(ourModel.meshes[0].vertices[index + 1].TexCoords.x, ourModel.meshes[0].vertices[index + 1].TexCoords.y, 0.0f, 1.0f),
              Vector4(ourModel.meshes[0].vertices[index + 2].TexCoords.x, ourModel.meshes[0].vertices[index + 2].TexCoords.y, 0.0f, 1.0f)
            };

            Matrix model = Matrix::Rotate(angley, 0.0f, 1.0f, 0.0f) * Matrix::Rotate(anglex, 1.0f, 0.0f, 0.0f) * preModel;
            preModel = model;

            Vector4 verticesView[3];
            Vector4 verticesClip[3];
            Vector4 verticesNDC[3];
            Vector4 verticesResult[3];

            int verticeNum = sizeof(vertices) / sizeof(vertices[0]);
            for (int i = 0; i < verticeNum; i++) {
                verticesView[i] = view * (model * vertices[i]);
            }

            for (int i = 0; i < verticeNum; i++) {
                verticesClip[i] = proj * verticesView[i];
            }

            for (int i = 0; i < verticeNum; i++) {
                verticesNDC[i] = verticesClip[i] / verticesClip[i].w;
            }

            for (int i = 0; i < verticeNum; i++) {
                verticesResult[i] = trans * (scale * verticesNDC[i]);
            }

            scan(verticesResult, verticesClip, 3, data, imagedata, verticesuv, width, height, zbuffer);

        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        GLuint fboId = 0;
        glGenFramebuffers(1, &fboId);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, texture1, 0);

        //render end
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // if not already bound
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    delete[] data;
    delete[] imagedata;
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (!startPress) {
   
        float xoffset = xpos - lastX;
        float yoffset = ypos - lastY;
        lastX = xpos;
        lastY = ypos;

        //axle.x = yoffset;
        //axle.y = xoffset;
        float ratio = 0.001f;
        anglex = std::sqrt(xoffset * xoffset) * ratio;
        angley = std::sqrt(yoffset * yoffset) * ratio;

    
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
 
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (startPress) {
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = xpos;
            lastY = ypos;
            startPress = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        if (!startPress) {
            startPress = true;
            anglex = 0.0f;
            angley = 0.0f;
        }
    }

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}