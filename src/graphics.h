#ifndef GRAPHICS
#define GRAPHICS

#include "inputTracking.h"

// Public method(s)
void initialiseOpenGL(void * procAddressFunction, ScreenSize * screenSize, char * modelName);
void renderOpenGL();
void releaseOpenGL();
void viewPortResizeCallback(ScreenSize * screenSize);
void mouseDragCallback(MousePosition * positions, ScreenSize * screenSize);
void scrollCallBack(ScrollPosition * positions);

#endif