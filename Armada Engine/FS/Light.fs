#version 330 core
out vec4 FragColor;
uniform sampler2D overlayTexture;

uniform vec4 lightPos;
uniform float lightRad;
uniform mat4 worldToView;
uniform mat4 viewToProjection;

uniform float nearPlaneDistance;
uniform float screenWidth;
uniform float screenHeight;

in vec2 uvCoord;
void main()
{
	vec4 adjustedLightPos = worldToView*lightPos;
	adjustedLightPos.z = adjustedLightPos.z - 2.0*nearPlaneDistance;
	vec4 lightProj = viewToProjection*adjustedLightPos;
	float depth = 1.0f/lightProj.z;

	vec4 range = (viewToProjection*vec4(1.0f,1.0f,0.0f,0.0f))*depth;

	vec4 uvScreen = (vec4(uvCoord-vec2(0.5f,0.5f),0.0f,2.0f))*2;

	vec4 span4 = (lightProj*depth)-uvScreen;
	vec2 span = vec2(span4.x/range.x, span4.y/range.y);
	float distance = sqrt(dot(span,span));

	float intensity = max(1.f-distance/lightRad,0.f);


	vec4 fc = texture(overlayTexture, uvCoord);
	fc.a = fc.a*intensity;

    FragColor = fc;
} 