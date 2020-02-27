#version 330 core
out vec4 FragColor;

in vec2 uvCoord;

void main()
{
    FragColor = vec4(uvCoord.x,uvCoord.y,1.0f,1.0f);
} 