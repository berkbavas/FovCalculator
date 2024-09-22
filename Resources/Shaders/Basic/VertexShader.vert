#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 fPosition;
out vec3 fNormal;

uniform mat4 nodeMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    fPosition = vec3(nodeMatrix * vec4(vPosition, 1.0));
    fNormal = mat3(transpose(inverse(nodeMatrix))) * vNormal;

    gl_Position = projectionMatrix * viewMatrix * vec4(fPosition, 1.0);
}
