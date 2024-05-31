#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "pixeldye.h"

#include <iostream>
#include "Vector4.h"
#include "Matrix.h"
#include "model.h"
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
float roll = 0.0f;
float pitch = 0.0f;
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

    Model ourModel("./assets/sphere.obj");

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    unsigned char* data = new unsigned char[3 * SCR_WIDTH * SCR_HEIGHT * sizeof(unsigned char)];
    float* zbuffer = new float[SCR_WIDTH * SCR_HEIGHT * sizeof(float)];

    Matrix preModel = Matrix::Scale(1.0f, 1.0f, 1.0f);
    Matrix view = Matrix::View(cameraPos, cameraTarget, up);
    Matrix proj = Matrix::Persp(fov, 0.1f, 100.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT);

    float a = (SCR_WIDTH) / 2.0;
    float b = (SCR_HEIGHT) / 2.0;

    Matrix scale = Matrix::Scale(a, b, 1.0f);
    Matrix trans = Matrix::Translate(a, b, 0.0f);

    int width, height, nrChannels;
    unsigned char* imagedata = stbi_load("./assets/default.png", &width, &height, &nrChannels, 0);

    std::vector<Vector4> allVerticesResult;
    std::vector<Vector4> allVerticesClip;
    std::vector<Vector4> allVerticesUV;

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
        Matrix model = Matrix::Rotate(roll, 1.0f, 0.0f, 0.0f) * Matrix::Rotate(pitch, 0.0f, 1.0f, 0.0f);


        for (int index = 0; index < ourModel.meshes[0].vertices.size(); index++) {

            Vector4 verticePos =  Vector4(ourModel.meshes[0].vertices[index].Position.x, ourModel.meshes[0].vertices[index].Position.y, ourModel.meshes[0].vertices[index].Position.z, 1.0f);
            Vector4 verticeUv = Vector4(ourModel.meshes[0].vertices[index].TexCoords.x, ourModel.meshes[0].vertices[index].TexCoords.y, 0.0f, 1.0f);
            Vector4 verticeView = view * (model * verticePos);
            Vector4 verticeClip = proj * verticeView;
            Vector4 verticeNDC = verticeClip / verticeClip.w;
            Vector4 verticeResult = trans * (scale * verticeNDC);

            allVerticesResult.push_back(verticeResult);
            allVerticesClip.push_back(verticeClip);
            allVerticesUV.push_back(verticeUv);
        }

        auto& indices = ourModel.meshes[0].indices;

        int size = indices.size();
        for (int i = 0; i < size / 3; i++) {
            int index = i * 3;
            Vector4 verticesResult[] = {
                Vector4(allVerticesResult[indices[index]].x, allVerticesResult[indices[index]].y, allVerticesResult[indices[index]].z, 1.0f),
                Vector4(allVerticesResult[indices[index + 1]].x, allVerticesResult[indices[index + 1]].y, allVerticesResult[indices[index + 1]].z, 1.0f),
                Vector4(allVerticesResult[indices[index + 2]].x, allVerticesResult[indices[index + 2]].y, allVerticesResult[indices[index + 2]].z, 1.0f),
            };

            Vector4 verticesClip[] = {
                Vector4(allVerticesClip[indices[index]].x, allVerticesClip[indices[index]].y, allVerticesClip[indices[index]].z, allVerticesClip[indices[index]].w),
                Vector4(allVerticesClip[indices[index + 1]].x, allVerticesClip[indices[index + 1]].y, allVerticesClip[indices[index + 1]].z, allVerticesClip[indices[index + 1]].w),
                Vector4(allVerticesClip[indices[index + 2]].x, allVerticesClip[indices[index + 2]].y, allVerticesClip[indices[index + 2]].z, allVerticesClip[indices[index + 2]].w),
            };

            Vector4 verticesuv[] = {
                Vector4(allVerticesUV[indices[index]].x, allVerticesUV[indices[index]].y, 0.0f, 1.0f),
                Vector4(allVerticesUV[indices[index + 1]].x, allVerticesUV[indices[index + 1]].y, 0.0f, 1.0f),
                Vector4(allVerticesUV[indices[index + 2]].x, allVerticesUV[indices[index + 2]].y, 0.0f, 1.0f),
            };

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

        allVerticesResult.clear();
        allVerticesClip.clear();
        allVerticesUV.clear();
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

        float ratio = 0.005f;
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
            lastX = xpos;
            lastY = ypos;
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