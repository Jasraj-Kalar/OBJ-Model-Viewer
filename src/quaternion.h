#ifndef QUATERNION
#define QUATERNION

#include <cglm/cglm.h>

#include "inputTracking.h"


// Public method(s)
void quaternionRotation(mat4 * modelMatrix, MousePosition * positions, ScreenSize * screen);

// Private method(s)
void screenCoordsToVector(int xCoord, int yCoord, ScreenSize * size, vec3 * v);

#endif