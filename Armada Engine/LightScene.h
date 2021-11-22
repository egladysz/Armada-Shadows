#pragma once
#include "SolidLight.h"
#include "Model.h"
#include "MaterialLightShadow.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>

class LightScene
{
public:
	LightScene(std::vector<SolidLight*> light, std::vector<Model*> model, MaterialLightShadow* shaders);
	~LightScene();

	void Render(glm::mat4 viewMat, GLID frameBufferID, GLID frameTextureID, unsigned int screenWidth, unsigned int screenHeight);
	void RenderObjects(glm::mat4 viewMat, unsigned int screenWidth, unsigned int screenHeight);
	glm::mat4 local;
private:
	MaterialLightShadow* shadowCaster;
	std::vector<SolidLight*> lights;
	std::vector<Model*> models;
};

