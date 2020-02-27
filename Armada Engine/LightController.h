#pragma once
#include "SolidLight.h"
#include <glm/glm/glm.hpp>
class LightController
{
	SolidLight* light;

	glm::vec3 initialPosition;
	glm::vec3 velocity;

	float initialOrbitRadian;
	glm::vec3 initialOrbitNormal;
	float orbitSpeedRadian;
	glm::vec3 orbitSpeedNormal;

	float initalBrightness;
	float deltaBrightness;



	glm::mat4 getCurrentLocalTransform(float currentTraversalTime) const;
};

