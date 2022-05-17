#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>

#include <vector>

#include "../Armada Engine/Shader.h"
#include "../Armada Engine/LightScene.h"



class Window
{
public:
	std::string title;
	GLFWwindow* glWindow;

	float cameraSpeed;



	Window(int width, int height, std::string header);
	bool initialize();
	void setCamera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up);
	~Window();
	void processInput(float deltaTime, bool & paused, int& scene);
	void render(std::vector<LightScene> scenes);
};

