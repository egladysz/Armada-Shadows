#include "LightScene.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <map>
#include <algorithm>
const float nearPlaneDistance = 0.1f;
const float farPlaneDistance = 10000.0f;

void LightScene::Render(glm::mat4 viewMat, GLID frameBufferID, GLID frameTextureID, unsigned int screenWidth, unsigned int screenHeight)
{
	static Vertex lightBox[4] = {
		{ glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f) },

		{ glm::vec3(1.0f,-1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f) },

		{ glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(-1.0f,-1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f) }
	};

	static Index lightIndex[6] =
	{
		0,1,3,
		2,0,3,
	};
	auto lightBox_p = std::make_unique<Vertex[]>(4);
	auto lightIndex_p = std::make_unique<Index[]>(6);

	std::copy(lightBox, lightBox + 4, lightBox_p.get());
	std::copy(lightIndex, lightIndex + 6, lightIndex_p.get());

	static Mesh lightScreen(std::move(lightBox_p), 4, std::move(lightIndex_p), 6);

	//clear frameBuffer to black in case there are no lights
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	for (SolidLight* light : lights) {
		if (!light->on) return;

		glm::vec3 lp = light->position;
		glm::vec4 lc = light->color;
		float lr = light->lightRadius;
		float ls = light->shadowLength;

		//fill frameBuffer with light's color
		
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		glBlendEquation(GL_MIN);
		glClearColor(lc.r, lc.g, lc.b, lc.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowCaster->use();
		shadowCaster->setLightParameters(*light, nearPlaneDistance);
		shadowCaster->setViewTransform(viewMat * local, glm::perspective(glm::radians(70.0f), (float)screenWidth / (float)screenHeight, nearPlaneDistance, farPlaneDistance));

		//draw shadows to cover light's color
		for (Model* m : models)
			m->Draw(*(shadowCaster->getShader()));

		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		glBindTexture(GL_TEXTURE_2D, frameTextureID);
		shadowCaster->useNext();
		shadowCaster->setLightParametersNext(*light, nearPlaneDistance);
		shadowCaster->setViewTransformNext(viewMat * local, glm::perspective(glm::radians(70.0f), (float)screenWidth / (float)screenHeight, nearPlaneDistance, farPlaneDistance));
		//draw shadowed texture to screen with appropriate brightness falloff
		lightScreen.Draw(*(shadowCaster->getShaderNext()));
	}
	glEnable(GL_DEPTH_TEST);
	
}

void LightScene::RenderObjects(glm::mat4 viewMat, unsigned int screenWidth, unsigned int screenHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//engage normal shader
	glDepthMask(GL_TRUE);
	//draw objects
	glDisable(GL_BLEND);
	
	for (Model* m : models) {
		m->material->use();
		m->material->setViewTransform(viewMat * local, glm::perspective(glm::radians(70.0f), (float)screenWidth / (float)screenHeight, nearPlaneDistance, farPlaneDistance));
		m->Draw(*(m->material->getShader()));
	}
}


LightScene::LightScene(std::vector<SolidLight*> light, std::vector<Model*> model, MaterialLightShadow* shaders)
{
	lights = light;
	models = model;
	shadowCaster = shaders;
	local = glm::mat4{ 1 };
}

LightScene::~LightScene()
{
}
