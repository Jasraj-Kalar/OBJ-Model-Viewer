#include <stdio.h>

#include <GLFW/glfw3.h>

#include "windowSystem.h"

#define APP_NAME "Model Viewer"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

static void windowSizeCallback(GLFWwindow* window, int width, int height);
static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

static GLFWwindow* window;
static viewportResizeFP viewportResizeFunctionPointer;
static mouseMovementFP mouseMovementFunctionPointer;
static mouseScrollFP mouseScrollFunctionPointer;
static ScreenSize screenSize = {.width = SCREEN_WIDTH, .height = SCREEN_HEIGHT};

void initialiseGLFW()
{
    if(!glfwInit())
        printf("Failed to initialise GLFW.\n");

    // Antialiasing
    glfwWindowHint(GLFW_SAMPLES, 4);

    // OpenGL version 4.1, newest working version on M1 Macbooks
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        APP_NAME, 
        NULL, 
        NULL
    );

    if(window == NULL)
    {
        printf("Failed to open GLFW window.\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Remove Apple specific IMK system logs
    #ifdef __APPLE__
        printf("\033[A\033[K");
        printf("\033[A\033[K");
        printf("\r");
    #endif
}

void * procAddressGLFW()
{
    return (void *)glfwGetProcAddress;
}

void processFrameGLFW(double elapsedTime, int fps)
{
    double startTime = glfwGetTime();
    glfwSwapBuffers(window);
    glfwPollEvents();

    // Sleep to limit FPS
    float frameTime = 1.0 / fps;
    double endTime = glfwGetTime();
    double deltaTime = endTime - startTime + elapsedTime;

    if (deltaTime < frameTime) 
    {
        double sleepTime = frameTime - deltaTime;
        
        // Handle events during sleep
        while (glfwGetTime() - endTime < sleepTime)
            glfwPollEvents();
    }
}

void processInputGLFW()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool applicationOpenGLFW()
{
    if (!glfwWindowShouldClose(window))
        return true;
    
    return false;
}

void releaseGLFW()
{
    glfwTerminate();
}

ScreenSize * getScreenSize()
{
    return &screenSize;
}

void initialiseWindowSizeCallbackGLFW(viewportResizeFP viewportResize)
{
    viewportResizeFunctionPointer = viewportResize;
    glfwSetFramebufferSizeCallback(window, windowSizeCallback);
}

void initialiseMouseMovementCallbackGLFW(mouseMovementFP mouseMovement)
{
    mouseMovementFunctionPointer = mouseMovement;
    glfwSetCursorPosCallback(window, cursorPositionCallback);
}

void initialiseMouseScrollCallbackGLFW(mouseScrollFP mouseScroll)
{
    mouseScrollFunctionPointer = mouseScroll;
    glfwSetScrollCallback(window, scroll_callback);
}

/*
    Fixed prototype of GLFW resize callback function
    Triggered in initialiseWindowSizeCallbackGLFW using glfwSetFramebufferSizeCallback
*/
void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    // Min screensize is 2px on M1 Macbook
    screenSize.width = width;
    screenSize.height = height;
    (*viewportResizeFunctionPointer)(&screenSize);
}

/*
    Fixed prototype of GLFW cursor position callback function
    Triggered in initialiseMouseMovementCallbackGLFW using glfwSetCursorPosCallback
*/
void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{   
    static MousePosition positions;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        positions.xPrevPosition = xPos;
        positions.yPrevPosition = yPos;
        return;
    }
        
    if (xPos < 0 || yPos < 0 || xPos > screenSize.width || yPos > screenSize.height)
        return;
    

    // Mouse is being dragged within the screen area
    positions.xPosition = xPos;
    positions.yPosition = yPos;

    (*mouseMovementFunctionPointer)(&positions, &screenSize);
}

/*
    Fixed prototype of GLFW scroll callback function
    Triggered in initialiseMouseScrollCallbackGLFW using glfwSetScrollCallback
*/
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    static ScrollPosition scrollPositions;

    scrollPositions.xOffset = xoffset;
    scrollPositions.yOffset = yoffset;

    (*mouseScrollFunctionPointer)(&scrollPositions);
}

