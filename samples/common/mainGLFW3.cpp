
#include <GLFW/glfw3.h>
#include "SampleApp.h"

#include <chrono>
#include <cstdarg>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main( int argc, char* args[] )
{
    SampleApp app;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(app.getScreenWidth(),
                                          app.getScreenHeight(),
                                          app.getSampleName(),
                                          nullptr, nullptr);

    if(!window)
    {
        return -1;
    }


    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    app.init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        int w, h;
        glfwGetWindowSize(window, &w, &h);
        double frameTime = glfwGetTime();

        app.render(w, h, frameTime);

        glfwSwapBuffers(window);
    }


    glfwTerminate();

    return 0;
}
