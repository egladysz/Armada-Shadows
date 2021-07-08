#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "Mesh.h"
class Model
{
public:
	Model();
	~Model();
	void Draw(const Shader shader) const;
	const Mesh* mesh;
	glm::mat4 transform;
};

