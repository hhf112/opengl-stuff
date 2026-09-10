#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
// in vec4 color;

// uniform mat4 uvTransform;
uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;
uniform sampler2D heightMap;

void main()
{    
    FragColor = texture(diffuseMap, TexCoords);
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

