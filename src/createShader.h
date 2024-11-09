#ifndef LOAD_SHADER
#define LOAD_SHADER


// Public method(s)
unsigned int createShaderProgram(char * vertexShaderPath, char * fragmentShaderPath);

// Private method(s)
char * loadShaderFromFile(char * filePath);
int compileAndAttachShader(char * shader, unsigned int shaderType);

#endif