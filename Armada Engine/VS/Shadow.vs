#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 uv;

uniform mat4 modelToWorld;
uniform mat4 worldToView;
uniform mat4 viewToProjection;


void main()
{
	gl_Position = worldToView * modelToWorld * vec4(pos, 1.0f);
}