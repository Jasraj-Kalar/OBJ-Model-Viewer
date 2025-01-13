#include "benchmark.h"
#include "graphics.h"
#include "windowSystem.h"

static double GLFWTime;
static double OpenGLTIME;
static int FPS = 60;

void groupRuntime()
{
    GLFWTime = benchmark(processInputGLFW);
    OpenGLTIME = benchmark(renderOpenGL);
    processFrameGLFW(GLFWTime + OpenGLTIME, 60);
}

void initialiseApplication(char * modelName)
{
    initialiseGLFW();
    initialiseWindowSizeCallbackGLFW(viewPortResizeCallback);
    initialiseMouseMovementCallbackGLFW(mouseDragCallback);
    initialiseMouseScrollCallbackGLFW(scrollCallBack);
    initialiseOpenGL(procAddressGLFW(), getScreenSize(), modelName);
}

void renderFrames()
{
    while(applicationOpenGLFW())
        benchmarkPrint(groupRuntime, "Main Loop");
}

void releaseResources()
{
    releaseGLFW();
    releaseOpenGL();

    printf("\r");
    fflush(stdout);
}

