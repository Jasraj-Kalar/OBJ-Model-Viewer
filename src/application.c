#include "graphics.h"
#include "windowSystem.h"

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
    {
        processInputGLFW();
        renderOpenGL();
        processFrameGLFW();
    }
}

void releaseResources()
{
    releaseGLFW();
    releaseOpenGL();
}
