#pragma once
#include <glm/glm/glm.hpp>
#include "Model.h"
class ModelController
{

	Model* model;

	glm::vec3 initialPosition;
	glm::vec3 velocity;

	float initialOrbitRadian;
	glm::vec3 initialOrbitNormal;
	float orbitSpeedRadian;
	glm::vec3 orbitSpeedNormal;

	glm::vec3 initialScale;
	glm::vec3 deltaScale;

	float initialOrientationRadian;
	glm::vec3 initialOrientationNormal;
	float rotationSpeedRadian;
	glm::vec3 rotationSpeedNormal;

	glm::mat4 getCurrentLocalTransform(float currentTraversalTime) const;


};

