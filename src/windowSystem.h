#ifndef WINDOW_SYSTEM
#define WINDOW_SYSTEM

#include <stdbool.h>
#include "inputTracking.h"

typedef void (*viewportResizeFP) (ScreenSize *);
typedef void (*mouseMovementFP) (MousePosition *, ScreenSize *);
typedef void (*mouseScrollFP) (ScrollPosition*);

// Public method(s)
void initialiseGLFW();
void initialiseWindowSizeCallbackGLFW(viewportResizeFP viewportResize);
void initialiseMouseMovementCallbackGLFW(mouseMovementFP mouseMovement);
void initialiseMouseScrollCallbackGLFW(mouseScrollFP mouseScroll);
void * procAddressGLFW();
void processFrameGLFW(double elapsedTime, int fps);
void processInputGLFW();
bool applicationOpenGLFW();
void releaseGLFW();
ScreenSize * getScreenSize();

#endif