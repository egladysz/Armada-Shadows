#include "LightScene.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <map>

void LightScene::injectViewTransform(Shader shader, glm::mat4 viewMat, unsigned int width, unsigned int height)
{
	glm::mat4 view;
	view = viewMat*local;
	glm::mat4 proj;
	proj = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 1.0f, 1000.0f);
	GLID viewLoc = glGetUniformLocation(shader.ID, "worldToView");
	GLID projLoc = glGetUniformLocation(shader.ID, "viewToProjection");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}
void LightScene::injectViewTransformNearAlign(Shader shader, glm::mat4 viewMat, unsigned int width, unsigned int height)
{
	float nearPlaneDistance = 0.1f;
	glm::mat4 view;
	view = viewMat * local;
	glm::mat4 proj;
	proj = glm::perspective(glm::radians(70.0f), (float)width / (float)height, nearPlaneDistance, 1000.0f);
	GLID viewLoc = glGetUniformLocation(shader.ID, "worldToView");
	GLID projLoc = glGetUniformLocation(shader.ID, "viewToProjection");
	GLID nearLoc = glGetUniformLocation(shader.ID, "nearPlaneDistance");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
	glUniform1f(nearLoc, nearPlaneDistance);
}

void LightScene::Render(glm::vec3 forward, glm::vec3 position, glm::mat4 viewMat, GLID frameBufferID, GLID frameTextureID, unsigned int screenWidth, unsigned int screenHeight)
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

	static Mesh lightScreen(lightBox, 4, lightIndex, 6);


	std::multimap<float, SolidLight*> sortedLights;
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		if (!lights[i]->on) {
			continue;
		}

		float distance = glm::dot(forward, (glm::vec3(local*glm::vec4(lights[i]->position,1.0f))) - position);
		if (distance >= 0)
		{
			sortedLights.insert(std::pair<float, SolidLight*>(distance, lights[i]));
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	for (std::multimap<float, SolidLight*>::reverse_iterator it = sortedLights.rbegin(); it != sortedLights.rend(); ++it)
	{
		glm::vec3 lp = it->second->position;
		glm::vec4 lc = it->second->color;
		float lr = it->second->lightRadius;
		float ls = it->second->shadowLength;
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		glBlendEquation(GL_MIN);
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
		glClearColor(lc.r, lc.g, lc.b, lc.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shadowStamp.use();
		shadowStamp.setVec4("lightPos", lp.x, lp.y, lp.z, 1.0f);
		shadowStamp.setVec4("lightColor", lc.r, lc.g, lc.b, lc.a);
		shadowStamp.setFloat("shadRad", ls);
		injectViewTransform(shadowStamp, viewMat, screenWidth,screenHeight);

		//draw objects
		for (int i = 0; i < models.size(); i++)
		{
			models[i]->Draw(shadowStamp);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		lightBlender.use();
		glBindTexture(GL_TEXTURE_2D, frameTextureID);
		lightBlender.setVec4("lightPos", lp.x, lp.y, lp.z, 1.0);
		lightBlender.setFloat("lightRad", lr);
		injectViewTransformNearAlign(lightBlender, viewMat, screenWidth, screenHeight);
		//draw texture
		lightScreen.Draw(lightBlender);
	}
	glEnable(GL_DEPTH_TEST);
	
}

void LightScene::RenderObjects(Shader objectShader, glm::mat4 viewMat, unsigned int screenWidth, unsigned int screenHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//engage normal shader
	objectShader.use();
	glDepthMask(GL_TRUE);
	injectViewTransform(objectShader, viewMat, screenWidth, screenHeight);
	//draw objects
	glDisable(GL_BLEND);
	for (int i = 0; i < models.size(); i++)
	{
		models[i]->Draw(objectShader);
	}
}


LightScene::LightScene(std::vector<SolidLight*> light, std::vector<Model*> model, Shader shadow, Shader blend)
{
	lights = light;
	models = model;
	shadowStamp = shadow;
	lightBlender = blend;
	local = glm::mat4{ 1 };
}

LightScene::~LightScene()
{
}
