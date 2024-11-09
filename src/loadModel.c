/******************************************************************************
 * File:        loadModel.c
 * Author:      Jasraj Kalar
 * Created:     2024-09-30
 * Description: This file contains the implementation of an OBJ file loader for
 *              integration with OpenGL. It handles OBJ files using 
 *              triangulation.
 *              Main functionalities include:
 *              - Capture vertices and normals from an OBJ file in the format 
 *                expected by OpenGL.
 *              - Perform scale calculations for the model.
 *              - Provide count of vertices for OpenGL functions.
 * Notes:       The loader only supports triangular mesh types.
 * License:     MIT License
 ******************************************************************************/


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loadModel.h"

#define TRIANGULAR_MESH_TYPE 3
#define ZOOM_LEVEL_CLOSE 6
#define ZOOM_LEVEL_MEDIUM 5
#define ZOOM_LEVEL_FAR 4

int loadModel(char * filename, float * scale, float ** vertices, float ** normals)
{
    int vertexCount;

    // Verify the file is an obj file
    if (!strcmp(&filename[strlen(filename) - 4], ".obj"))
        vertexCount = loadOBJ(filename, scale, vertices, normals);

    return vertexCount;
}

/*
    OBJ Format

    v       Vertex      xCoord yCoord zCoord                (Position Coordinates)
    vt      Texture     uCoord vCoord                       (Not used)
    vn      Normals     xCoord yCoord zCoord                (Direction of each normal)
    f       Faces       v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3    (Specify each triangle)

    Support is limited to OBJ files using triangulation.
*/

int loadOBJ(char * filename, float * scale, float ** vertices, float ** normals)
{
    int vertexCount     = 0;
    int normalCount     = 0;
    int faceCount       = 0;

    // LoadOBJ is currently limited to triangular mesh types
    int meshType = TRIANGULAR_MESH_TYPE;

    FILE *input = fopen(filename, "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return -1;
    }

    char line[256];

    // Capture the number of vertices, normals and faces in the obj file
    while(fgets(line, sizeof(line), input) != NULL)
    {
        if (line[0] == 'v' && line[1] == ' ')
            vertexCount++;
        else if (line[0] == 'v' && line[1] == 'n')
            normalCount++;
        else if (line[0] == 'f')
            faceCount++;
    }

    float * uniqueVertices = (float *)malloc(sizeof(float) * vertexCount * meshType);
    float * uniqueNormals = (float *)malloc(sizeof(float) * normalCount * meshType);
    int * faceVertices = (int *)malloc(sizeof(int) * faceCount * meshType);
    int * faceNormals = (int *)malloc(sizeof(int) * faceCount * meshType);

    // Reset file pointer to top of file
    fseek(input, 0, SEEK_SET);

    // Reset counters for additional computations
    vertexCount     = 0;
    normalCount     = 0;
    faceCount       = 0;
    
    float xLargest = 0.0;
    float yLargest = 0.0;
    float zLargest = 0.0;

    float xSmallest = 0.0;
    float ySmallest = 0.0;
    float zSmallest = 0.0;

    // Store file components in memory
    while(fgets(line, sizeof(line), input) != NULL)
    {
        if (line[0] == 'v' && line[1] == ' ')
        {
            sscanf(line, "v %f %f %f\n",
                &uniqueVertices[vertexCount],
                &uniqueVertices[vertexCount + 1],
                &uniqueVertices[vertexCount + 2]
            );

            if (uniqueVertices[vertexCount] > xLargest)
                xLargest = uniqueVertices[vertexCount];
            if (uniqueVertices[vertexCount + 1] > yLargest)
                yLargest = uniqueVertices[vertexCount + 1];
            if (uniqueVertices[vertexCount + 2] > zLargest)
                zLargest = uniqueVertices[vertexCount + 2];

            if (uniqueVertices[vertexCount] < xSmallest)
                xSmallest = uniqueVertices[vertexCount];
            if (uniqueVertices[vertexCount + 1] < ySmallest)
                ySmallest = uniqueVertices[vertexCount + 1];
            if (uniqueVertices[vertexCount + 2] < zSmallest)
                zSmallest = uniqueVertices[vertexCount + 2];

            vertexCount += 3;
        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            sscanf(line, "vn %f %f %f\n",
                &uniqueNormals[normalCount],
                &uniqueNormals[normalCount + 1],
                &uniqueNormals[normalCount + 2] 
            );

            normalCount += 3;
        }
        else if (line[0] == 'f')
        {
            // Store the first and third position for each vertex
            // Ignore the texture position
            sscanf(line, "f %d/%*d/%d %d/%*d/%d %d/%*d/%d\n",
                &faceVertices[faceCount],
                &faceNormals[faceCount],
                &faceVertices[faceCount + 1],
                &faceNormals[faceCount + 1],
                &faceVertices[faceCount + 2],
                &faceNormals[faceCount + 2]
            );

            // Since face numbers start from one, one is subtracted to match the
            // programs convention of starting from zero
            faceVertices[faceCount] -= 1;
            faceNormals[faceCount] -= 1;
            faceVertices[faceCount + 1] -= 1;
            faceNormals[faceCount + 1] -= 1;
            faceVertices[faceCount + 2] -= 1;
            faceNormals[faceCount + 2] -= 1;

            faceCount += 3;
        }
    }

    *vertices = (float *)malloc(sizeof(float) * meshType * faceCount);
    *normals = (float *)malloc(sizeof(float) * meshType * faceCount);

    vertexCount = 0;
    normalCount = 0;

    float xCenter = (xLargest + xSmallest) / 2;
    float yCenter = (yLargest + ySmallest) / 2;
    float zCenter = (zLargest + zSmallest) / 2;

    // Compute largest overall coordinate for scale calculation
    float largestCoord = 0.0;

    if ((fabs(xLargest) + fabs(xSmallest)) > largestCoord)
        largestCoord = fabs(xLargest) + fabs(xSmallest);
    if ((fabs(yLargest) + fabs(ySmallest)) > largestCoord)
        largestCoord = fabs(yLargest) + fabs(ySmallest);
    if ((fabs(zLargest) + fabs(zSmallest)) > largestCoord)
        largestCoord = fabs(zLargest) + fabs(zSmallest);

    *scale = ZOOM_LEVEL_FAR / largestCoord;

    for (int i = 0; i < faceCount; i++)
    {
        // Subtract center coordinate to center object at origin
        int faceVertexIndex = 3 * faceVertices[i];
        (*vertices)[vertexCount]     = uniqueVertices[faceVertexIndex] - xCenter;
        (*vertices)[vertexCount + 1] = uniqueVertices[faceVertexIndex + 1] - yCenter;
        (*vertices)[vertexCount + 2] = uniqueVertices[faceVertexIndex + 2] - zCenter;

        int faceNormalIndex = 3 * faceNormals[i];
        (*normals)[normalCount]       = uniqueNormals[faceNormalIndex];
        (*normals)[normalCount + 1]   = uniqueNormals[faceNormalIndex + 1];
        (*normals)[normalCount + 2]   = uniqueNormals[faceNormalIndex + 2];

        vertexCount += 3;
        normalCount += 3;
    }

    free(uniqueVertices);
    free(uniqueNormals);
    free(faceVertices);
    free(faceNormals);

    return vertexCount;
}