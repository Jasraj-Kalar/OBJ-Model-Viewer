#include <stdio.h>

#include <cglm/cglm.h>
#include <glad/glad.h>

#include "createShader.h"
#include "getGLErrors.h"
#include "graphics.h"
#include "loadModel.h"
#include "quaternion.h"

static unsigned int VBO;
static unsigned int VAO;
static unsigned int NBO;
static mat4 proj;
static mat4 view;
static mat4 model;
static mat4 mvp;
static mat3 normalMatrix;
static vec3 lightPosition;
static vec3 lightColor;
static vec3 modelColor;
static vec3 cameraPosition;
static ScreenSize *screenPtr;
static float scale = 4.0;
static float *vertices;
static float *normals;
static float reflectance;
static int vertexCount = 36;
static int shaderProgram;
static int uniformLocationMVP;
static int uniformLocationModel;
static int uniformLocationLightPosition;
static int uniformLocationLightColor;
static int uniformLocationModelColor;
static int uniformLocationModelReflectance;
static int uniformLocationCameraPosition;
static int uniformLocationNormalMatrix;

void initialiseOpenGL(void *procAddressFunction, ScreenSize *screenSize, char *modelName)
{
    vertexCount = loadModel(modelName, &scale, &vertices, &normals);

    screenPtr = screenSize;

    if (!gladLoadGLLoader((GLADloadproc)procAddressFunction))
        printf("Failed to initialise GLAD.\n");

    shaderProgram = createShaderProgram("src/res/shaders/vertex.shader", "src/res/shaders/fragment.shader");
    glUseProgram(shaderProgram);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // Setup positions
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    GET_GL_ERRORS();

    // Setup Normals 
    glGenBuffers(1, &NBO);
    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    GET_GL_ERRORS();

    // Unbind VAO and buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    GET_GL_ERRORS();

    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(proj);

    glm_vec3_copy((vec3){0.0f, 0.0f, 10.0f}, cameraPosition);
    glm_perspective(glm_rad(45.0f), screenPtr->width / screenPtr->height, 0.1, 100, proj);
    glm_lookat(cameraPosition, (vec3){0, 0, 0}, (vec3){0, 1, 0}, view);
    glm_translate(model, (vec3){0, 0, 0});
    glm_scale(model, (vec3){scale, scale, scale});
    glm_mat4_mulN((mat4 *[]){&proj, &view, &model}, 3, mvp);

    glm_mat4_pick3(model, normalMatrix);
    glm_mat3_inv(normalMatrix, normalMatrix);
    glm_mat3_transpose(normalMatrix);

    glm_vec3_copy((vec3){0.5f, 0.5f, 0.5f}, modelColor);
    glm_vec3_copy((vec3){0.8f, 0.1f, 0.2f}, lightColor);
    glm_vec3_copy((vec3){5.0f, 5.0f, 10.0f}, lightPosition);

    // Lower values yeild more reflectance. 80 = Medium reflectance
    reflectance = 80.0;

    uniformLocationMVP = glGetUniformLocation(shaderProgram, "MVP");
    uniformLocationModel = glGetUniformLocation(shaderProgram, "model");
    uniformLocationModelColor = glGetUniformLocation(shaderProgram, "modelColor");
    uniformLocationModelReflectance = glGetUniformLocation(shaderProgram, "modelReflectance");
    uniformLocationLightColor = glGetUniformLocation(shaderProgram, "lightColor");
    uniformLocationLightPosition = glGetUniformLocation(shaderProgram, "lightPosition");
    uniformLocationCameraPosition = glGetUniformLocation(shaderProgram, "cameraPosition");
    uniformLocationNormalMatrix = glGetUniformLocation(shaderProgram, "normalMatrix");

    glUniformMatrix4fv(uniformLocationMVP, 1, GL_FALSE, (float *)mvp);
    glUniformMatrix4fv(uniformLocationModel, 1, GL_FALSE, (float *)model);
    glUniformMatrix3fv(uniformLocationNormalMatrix, 1, GL_FALSE, (float *)normalMatrix);
    glUniform3fv(uniformLocationModelColor, 1, (float *)modelColor);
    glUniform3fv(uniformLocationLightColor, 1, (float *)lightColor);
    glUniform3fv(uniformLocationLightPosition, 1, (float *)lightPosition);
    glUniform3fv(uniformLocationCameraPosition, 1, (float *)cameraPosition);
    glUniform1f(uniformLocationModelReflectance, reflectance);
    GET_GL_ERRORS();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    GET_GL_ERRORS();
}

void renderOpenGL()
{
    // MVP is updated due to screensize changes impacting the projection matrix
    glm_perspective(glm_rad(45.0f), screenPtr->width / screenPtr->height, 0.1, 100, proj);
    glm_mat4_mulN((mat4 *[]){&proj, &view, &model}, 3, mvp);
    glm_mat4_pick3(model, normalMatrix);
    glm_mat3_inv(normalMatrix, normalMatrix);
    glm_mat3_transpose(normalMatrix);
    glUniformMatrix4fv(uniformLocationModel, 1, GL_FALSE, (float *)model);
    glUniformMatrix4fv(uniformLocationMVP, 1, GL_FALSE, (float *)mvp);
    glUniformMatrix3fv(uniformLocationNormalMatrix, 1, GL_FALSE, (float *)normalMatrix);
    
    GET_GL_ERRORS();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    GET_GL_ERRORS();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GET_GL_ERRORS();
    glBindVertexArray(VAO);
    GET_GL_ERRORS();

    glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);
    GET_GL_ERRORS();
    glBindVertexArray(0);
    GET_GL_ERRORS();
}

void releaseOpenGL()
{
    // TODO is everything being free'd?
    free(vertices);
    free(normals);
}

void viewPortResizeCallback(ScreenSize *screenSize) { glViewport(0, 0, screenSize->width, screenSize->height); }

void mouseDragCallback(MousePosition *positions, ScreenSize *screenSize)
{
    quaternionRotation(&model, positions, screenSize);
}

void scrollCallBack(ScrollPosition *positions)
{
    float zoomInFactor = 1.05, zoomOutFactor = 0.95;

    if (positions->yOffset > 0)
        glm_scale(model, (vec3){zoomInFactor, zoomInFactor, zoomInFactor});
    else if (positions->yOffset < 0)
        glm_scale(model, (vec3){zoomOutFactor, zoomOutFactor, zoomOutFactor});
}
