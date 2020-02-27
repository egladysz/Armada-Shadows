#version 330 core
out vec4 FragColor;
in float shadowAlpha;

uniform vec4 lightColor;

void main()
{
    FragColor = vec4(lightColor.r,lightColor.g,lightColor.b,lightColor.a*shadowAlpha);
} 