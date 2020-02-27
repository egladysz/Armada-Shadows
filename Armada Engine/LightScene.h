#pragma once
#include "SolidLight.h"
#include "Model.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>

class LightScene
{
public:
	LightScene(std::vector<SolidLight*> light, std::vector<Model*> model, Shader shadow, Shader blend);
	~LightScene();

	void Render(glm::vec3 forward, glm::vec3 position, glm::mat4 viewMat, GLID frameBufferID, GLID frameTextureID, unsigned int screenWidth, unsigned int screenHeight);
	void RenderObjects(Shader objectShader, glm::mat4 viewMat, unsigned int screenWidth, unsigned int screenHeight);
	glm::mat4 local;
private:
	Shader shadowStamp;
	Shader lightBlender;
	std::vector<SolidLight*> lights;
	std::vector<Model*> models;

	void injectViewTransform(Shader shader, glm::mat4 viewMat, unsigned int width, unsigned int height);
	void injectViewTransformNearAlign(Shader shader, glm::mat4 viewMat, unsigned int width, unsigned int height);
};

