#version 330 core
out vec4 FragColor;

in vec2 lightPoint;
in vec2 uvCoord;
void main()
{
	vec2 displacement = uvCoord - lightPoint;
	float squareDistance = dot(displacement,displacement);
	float intensity = min(1.0f,abs(1.0f/squareDistance));
    FragColor = vec4(1.0,1.0f,1.0f,1.0);
} 