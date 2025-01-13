#version 330 core

// Vertices and Normals from OBJ file
layout (location = 0) in vec3 vertexBuffer;
layout (location = 1) in vec3 normalBuffer;

// Information transfer from C code
uniform mat4 MVP;
uniform mat4 model;
uniform mat3 normalMatrix;

// Output to Fragment Shader
out vec3 normals;
out vec3 vertices;

void main()
{
    vec4 worldPosition = vec4(vertexBuffer, 1.0);
    vertices = vec3(model * worldPosition);

    normals = normalMatrix * normalBuffer;
    
    gl_Position = MVP * worldPosition;
}