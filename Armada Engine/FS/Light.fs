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

	vec2 range = (viewToProjection*vec4(1.0f,1.0f,0.0f,0.0f)).xy*depth;

	vec2 uvScreen = (uvCoord-vec2(0.5f,0.5f))*2;

	vec2 span = ((lightProj.xy*depth)-uvScreen)/range;
	float sqDistance = dot(span,span);

	float intensity = 1.f - min(sqDistance/(lightRad*lightRad),1.f);


	vec4 fc = texture(overlayTexture, uvCoord);
	fc.a *= intensity;

    FragColor = fc;
} 