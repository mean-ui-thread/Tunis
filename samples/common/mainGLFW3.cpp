
#include "SampleApp.h"

#include <GLFW/glfw3.h>

#include <sstream>

#include <easy/profiler.h>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main( int argc, char* args[] )
{
    EASY_MAIN_THREAD;
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

    GLFWwindow* window = glfwCreateWindow(SampleApp::getWindowWidth(),
                                          SampleApp::getWindowHeight(),
                                          "", nullptr, nullptr);

    if(!window)
    {
        return -1;
    }


    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    {
        // tunis::Context can only be instantiated after a context is created
        // and made current
        SampleApp app;

        std::stringstream title;
        title << SampleApp::getSampleName() << " - " << app.ctx.backendName();

        glfwSetWindowTitle(window, title.str().c_str());

        std::string frameName = std::string("Frame with ") + app.ctx.backendName() + " backend";
        while (!glfwWindowShouldClose(window))
        {
            EASY_BLOCK(frameName);
            glfwPollEvents();

            int winWidth, winHeight, fbWidth , fbHeight;
            glfwGetWindowSize(window, &winWidth, &winHeight);
            glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

            // Calculate pixel ration for hi-dpi devices.
            float pxRatio = static_cast<float>(fbWidth) / static_cast<float>(winWidth);

            double frameTime = glfwGetTime();
            app.ctx.clearFrame(0, 0, fbWidth, fbHeight);
            app.ctx.beginFrame(winWidth, winHeight, pxRatio);
            app.render(frameTime);
            app.ctx.endFrame();
            EASY_END_BLOCK;

            glfwSwapBuffers(window);
        }
    }


    glfwTerminate();

    profiler::dumpBlocksToFile("easy_profiler.prof");

    return 0;
}
