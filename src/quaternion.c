/*
*       Quaternion mathematics from Ken Shoemake - ARCBALL: a user interface 
*       for specifying three-dimensional orientation using a mouse
*/


#include <math.h>
#include <stdio.h>

#include <cglm/cglm.h>

#include "quaternion.h"

void quaternionRotation(mat4 * modelMatrix, MousePosition * positions, ScreenSize * size)
{
    vec3 oldVector, newVector;

    screenCoordsToVector(positions->xPrevPosition, positions->yPrevPosition, size, &oldVector);
    screenCoordsToVector(positions->xPosition, positions->yPosition, size, &newVector);

    glm_vec3_normalize(oldVector);
    glm_vec3_normalize(newVector);

    vec3 axisOfRotation;
    
    glm_vec3_cross(oldVector, newVector, axisOfRotation);
    float angle = glm_vec3_angle(oldVector, newVector);

    float rotationSpeed = size->width / 80.0 + size->height / 60.0;
    angle *= rotationSpeed;

    vec4 quaternion = {
        sin(angle / 2) * axisOfRotation[0],
        sin(angle / 2) * axisOfRotation[1], 
        sin(angle / 2) * axisOfRotation[2],
        cos(angle / 2)
    };

    glm_vec4_normalize(quaternion);

    mat4 quaternionMat;
    glm_quat_mat4(quaternion, quaternionMat);

    glm_mat4_mul(quaternionMat, *modelMatrix, *modelMatrix);

    positions->xPrevPosition = positions->xPosition;
    positions->yPrevPosition = positions->yPosition;
}

void screenCoordsToVector(int xCoord, int yCoord, ScreenSize * size, vec3 * v)
{
    float min = fmin(size->width, size->height) - 1;

    (*v)[0] = (2.0 * xCoord - size->width) / min;
    (*v)[1] = (size->height - 2.0 * yCoord) / min;

    if ((*v)[0] * (*v)[0] + (*v)[1] * (*v)[1] <= 0.5)
        (*v)[2] = sqrt(1 - (*v)[0] * (*v)[0] - (*v)[1] * (*v)[1]);
    else
        (*v)[2] = 0.5 / sqrt((*v)[0] * (*v)[0] + (*v)[1] * (*v)[1]);
}
