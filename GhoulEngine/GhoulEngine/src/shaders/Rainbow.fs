#version 330 core
out vec4 FragColor;

in vec3 ourColor;

uniform float redVal;
uniform float greenVal;
uniform float blueVal;

void main()
{
    FragColor = vec4(redVal, greenVal, blueVal, 1.0f);
}