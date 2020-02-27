#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

uniform vec4 lightPos;
uniform mat4 worldToView;
uniform mat4 viewToProjection;


out vec2 lightPoint;
out vec2 uvCoord;
void main()
{
	vec4 lightScreen = viewToProjection * worldToView  * lightPos;
	gl_Position = vec4(pos.x, pos.y, lightScreen.z, 1.0);
	lightPoint = vec2(lightScreen.x, lightScreen.y);
	uvCoord = uv;
}