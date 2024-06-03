#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "Vector4.h"
#include "Matrix.h"
#include "model.h"
#include <stb_image.h>
#include "Pipeline.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);

//camera
Vector4 cameraPos(0.0f, 0.0f, 7.0f, 1.0f);
Vector4 cameraTarget(0.0f, 0.0f, 0.0f, 1.0f);
Vector4 up(0.0f, 1.0f, 0.0f, 0.0f);

//light
float intensity = 1.0f;
Vector4 lightDir(0.0f, 0.0f, 1.0f, 0.0f);
Vector4 lightColor(1.0f, 1.0f, 1.0f, 0.0f);

bool startPress = true;
float roll = 0.0;
float pitch = 0.0;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
Vector4 axle(0.0f, 0.0f, 0.0f, 0.0f);
float deltaTime = 0.0f;
float lastFrame = 0.0f;
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

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
    
    Model ourModel("./assets/backpack/backpack.obj");

    Matrix view = Matrix::View(cameraPos, cameraTarget, up);
    Matrix proj = Matrix::Persp(fov, 0.1f, 100.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT);

    ShadingConstants constants;
    constants.cameraPos = cameraPos;
    constants.viewMatrix = view;
    constants.projectionMatrix = proj;
    constants.lightColor = lightColor;
    constants.lightDir = lightDir;
    constants.mainTex = new Texture2D("./assets/backpack/diffuse.jpg");
    constants.intensity = intensity;

    Pipeline pipeline;
    pipeline.Initialize(SCR_WIDTH, SCR_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----

        processInput(window);

        //render
        pipeline.Clear();
        Matrix model = Matrix::Rotate(roll, 1.0f, 0.0f, 0.0f) * Matrix::Rotate(pitch, 0.0f, 1.0f, 0.0f);
        constants.modelMatrix = model;
        //
        for (int i = 0; i < ourModel.meshes.size(); i++)
        {
            pipeline.DrawCall(ourModel.meshes[i].vertices, ourModel.meshes[i].indices, constants);

        }

        pipeline.Present();
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    delete constants.mainTex;
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

        float ratio = 0.3f;
        pitch += xoffset * ratio;
        roll += yoffset * ratio;

    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
 
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (startPress) {
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);
            startPress = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        if (!startPress) {
            startPress = true;

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