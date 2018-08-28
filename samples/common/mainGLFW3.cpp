
#include <GLFW/glfw3.h>
#include "SampleApp.h"

#include <chrono>
#include <cstdarg>

#include <easy/profiler.h>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main( int argc, char* args[] )
{
    EASY_PROFILER_ENABLE;
    profiler::startListen();

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    GLFWwindow* window = glfwCreateWindow(SampleApp::getScreenWidth(),
                                          SampleApp::getScreenHeight(),
                                          SampleApp::getSampleName(),
                                          nullptr, nullptr);

    if(!window)
    {
        return -1;
    }


    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // tunis::Context can only be instantiated after a context is created.
    SampleApp app;

    while (!glfwWindowShouldClose(window))
    {
        EASY_BLOCK("Events Polling")
        glfwPollEvents();
        EASY_END_BLOCK

        EASY_BLOCK("Application Rendering",  profiler::colors::Blue500)
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        double frameTime = glfwGetTime();
        app.render(w, h, frameTime);
        EASY_END_BLOCK

        EASY_BLOCK("Swap Buffer", profiler::colors::Cyan)
        glfwSwapBuffers(window);
        EASY_END_BLOCK
    }


    glfwTerminate();

    return 0;
}
