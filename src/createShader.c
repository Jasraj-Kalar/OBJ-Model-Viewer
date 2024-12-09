#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

#include "createShader.h"


unsigned int shaderProgramID = 0;

// Public method(s)

unsigned int createShaderProgram(char * vertexShaderPath, char * fragmentShaderPath)
{
    char * vertexShaderSource = loadShaderFromFile(vertexShaderPath);
    char * fragmentShaderSource = loadShaderFromFile(fragmentShaderPath);

    shaderProgramID = glCreateProgram();

    // An OpenGL specific shader type must be specified
    int vertexShader = compileAndAttachShader(vertexShaderSource, GL_VERTEX_SHADER);
    int fragmentShader = compileAndAttachShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    // Free memory allocation during loadShaderFromFile()
    free(vertexShaderSource);
    free(fragmentShaderSource);

    glLinkProgram(shaderProgramID);

    int status;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &status);

    char linkStatus[512];

    if (!status)
    {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, linkStatus);
        printf("Shader linking error: %s\n", linkStatus);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgramID;
}

// Private method(s)

char * loadShaderFromFile(char * filePath)
{
    FILE * input = fopen(filePath, "rb");

    if (input == NULL)
    {
        printf("FILE PATH: %s\n", filePath);
        printf("Could not open file.\n");
        return NULL;
    }

    // Determine the size of the file
    fseek(input, 0, SEEK_END);
    long fileSize = ftell(input);
    fseek(input, 0, SEEK_SET);

    char * shaderSource = (char *) malloc(fileSize + 1);

    if (shaderSource == NULL)
    {
        printf("Shader memory allocation error.\n");
        fclose(input);
        return NULL;
    }

    if (fread(shaderSource, 1, fileSize, input) != fileSize)
    {
        printf("%s\n", shaderSource);
        printf("Error reading shader.\n");
        free(shaderSource);
        fclose(input);
        return NULL;
    }
    
    fclose(input);

    shaderSource[fileSize] = '\0';

    return shaderSource;
}

int compileAndAttachShader(char * shaderSource, unsigned int shaderType)
{
    unsigned int shaderID = glCreateShader(shaderType);

    glShaderSource(shaderID, 1, (char const * const *)&shaderSource, NULL);

    glCompileShader(shaderID);

    int status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    char compilationStatus[512];

    if (!status)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, compilationStatus);
        printf("Shader compilation error: %s\n%s\n", shaderSource, compilationStatus);
    }
        
    glAttachShader(shaderProgramID, shaderID);

    return shaderID;
}