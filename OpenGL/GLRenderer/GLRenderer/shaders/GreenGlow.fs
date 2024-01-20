#version 330 core
out vec4 FragColor;

in vec3 ourColor;

uniform float greenVal;

void main()
{
    FragColor = vec4(0.0f, greenVal, 0.0f, 1.0f);
}