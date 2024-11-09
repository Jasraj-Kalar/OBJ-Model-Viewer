#ifndef QUATERNION
#define QUATERNION

#include <cglm/cglm.h>

#include "inputTracking.h"


// Public method(s)
void quaternionRotation(mat4 * modelMatrix, MousePosition * positions, ScreenSize * screen);


#endif