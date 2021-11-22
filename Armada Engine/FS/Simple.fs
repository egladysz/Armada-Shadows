#version 330 core
out vec4 FragColor;

in vec2 uvCoord;
uniform vec4 color;
void main()
{
    FragColor = vec4(color.rgb, 1.0f);
} 