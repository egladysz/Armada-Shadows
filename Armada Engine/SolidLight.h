#pragma once
#include <glm/glm/glm.hpp>
class SolidLight
{
public:
	SolidLight();
	~SolidLight();
	glm::vec3 position;
	glm::vec4 color;
	float lightRadius;
	float shadowLength;
	bool on;
};

