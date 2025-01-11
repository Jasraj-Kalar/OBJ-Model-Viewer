#version 330 core

// Information from Vertex Shader
in vec3 vertices;
in vec3 normals;

// Output to Frame Buffer
out vec4 FinalFragmentColor;

// Information transfer from C code
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 modelColor;
uniform vec3 cameraPosition;
uniform float modelReflectance;

void main()
{
    // Ambient lighting
    vec3 ambient = 0.7 * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(normals);
    vec3 lightDir = normalize(lightPosition - vertices);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    vec3 viewDir = normalize(cameraPosition - vertices);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), modelReflectance);
    vec3 specular = spec * lightColor;

    // Combine lighting componenets
    vec3 absoluteColor = (ambient + diffuse + specular) * modelColor;

    // Set the final fragment color
    FinalFragmentColor = vec4(absoluteColor, 1.0);
}