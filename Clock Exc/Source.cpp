#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Window.h"
#include "../Armada Engine/Shader.h"
#include "../Armada Engine/Mesh.h"
#include "../Armada Engine/Model.h"
#include "../Armada Engine/SolidLight.h"
#include "../Armada Engine/LightScene.h"
#include "../Armada Engine/MeshReaderObj.h"
#include <chrono>
unsigned int screenWidth = 320;
unsigned int screenHeight = 120;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool paused = false;




int main()
{
	Window window(screenWidth, screenHeight, "Exception.EXE Clock");
	//GLFW setup
	if (!window.initialize())
	{
		return -1;
	}

	ShaderElement simpleVS("#version 330 core\nlayout(location = 0) in vec3 pos;\nlayout(location = 1) in vec3 norm;\nlayout(location = 2) in vec2 uv;\nuniform mat4 modelToWorld;\n\
							uniform mat4 worldToView;\nout vec2 uvCoord;\nvoid main()\n{\n\tuvCoord = uv;\n\tgl_Position = worldToView * modelToWorld * vec4(pos, 1.0f);\n}", GL_VERTEX_SHADER, true);

	ShaderElement simpleFS("#version 330 core\nout vec4 FragColor;\nin vec2 uvCoord;\nuniform vec4 color;\nvoid main()\n{\n\tFragColor = color;\n} ", GL_FRAGMENT_SHADER, true);

	Shader simpleShader;

	simpleShader.addElement(simpleVS);
	simpleShader.addElement(simpleFS);
	simpleShader.prime();
	MeshReaderObj meshReader;

	static glm::vec4 colorIndex[16]{
		glm::vec4(0.0f,0.0f,0.0f,1.0f),		//black
		glm::vec4(0.5f,0.0f,0.0f,1.0f),		//maroon
		glm::vec4(0.0f,0.5f,0.0f,1.0f),		//green
		glm::vec4(0.5f,0.5f,0.0f,1.0f),		//olive
		glm::vec4(0.0f,0.0f,0.5f,1.0f),		//navy
		glm::vec4(0.5f,0.0f,0.5f,1.0f),		//purple
		glm::vec4(0.0f,0.5f,0.5f,1.0f),		//teal
		glm::vec4(0.75f,0.75f,0.75f,1.0f),	//silver
		glm::vec4(0.5f,0.5f,0.5f,1.0f),		//gray
		glm::vec4(1.0f,0.0f,0.0f,1.0f),		//red
		glm::vec4(0.0f,1.0f,0.0f,1.0f),		//lime
		glm::vec4(1.0f,1.0f,0.0f,1.0f),		//yellow
		glm::vec4(0.0f,0.0f,1.0f,1.0f),		//blue
		glm::vec4(1.0f,0.0f,1.0f,1.0f),		//fuchsia
		glm::vec4(0.0f,1.0f,1.0f,1.0f),		//aqua
		glm::vec4(1.0f,1.0f,1.0f,1.0f),		//white
	};


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

	static Mesh square(std::move(lightBox_p), 4, std::move(lightIndex_p), 6);



	//Agents that updates time-dependant components
	std::vector<Model> models;

	//Section 1: Current Epoch



	for (int i = 0; i < 8; i++)
	{
		Model sq;
		sq.mesh = &square;
		sq.transform = glm::scale(glm::translate(glm::mat4{ 1 }, glm::vec3(7.5f - i, 0.f, 0.f)), glm::vec3(0.5f, 0.5f, 1.0f));
		models.push_back(sq);
	}

	for (int j = 0; j < 4; j++)
	{
		Model sq;
		sq.mesh = &square;
		sq.transform = glm::scale(glm::translate(glm::mat4{ 1 }, glm::vec3(7.f - 2 * j, 1.5f, 0.f)), glm::vec3(1.0f, 1.0f, 1.0f));
		models.push_back(sq);
	}

	const auto epochStart = std::chrono::time_point<std::chrono::system_clock>{};
	auto timerStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - epochStart).count();

	while (!glfwWindowShouldClose(window.glWindow))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input
		bool shouldReset = false;
		window.processInput(shouldReset);
		if (shouldReset)
			timerStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - epochStart).count();

		std::vector<glm::vec4> nowColors{};

		static long long lastMoment;
		static float timeValue = static_cast<float>(glfwGetTime());
		timeValue += deltaTime;
		auto now = std::chrono::system_clock::now();
		auto epochTime = std::chrono::duration_cast<std::chrono::seconds>(now - epochStart).count();
		if (lastMoment != epochTime) { // its a new second
			for (int i = 0; i < 8; i++) {
				nowColors.push_back(colorIndex[(epochTime >> (4 * i)) % 16]);
			}
			auto timer = std::chrono::duration_cast<std::chrono::seconds>(now - epochStart).count() - timerStart;
			for (int j = 0; j < 4; j++) {
				nowColors.push_back(colorIndex[(timer >> (4 * j)) % 16]);
			}
			window.render(models, simpleShader, nowColors);
		}
		lastMoment = epochTime;

		//render

		// 0. copy our vertices array in a buffer for OpenGL to use
		// 1. then set the vertex attributes pointers
		// 2. use our shader program when we want to render an object

		//window.render(models, simpleShader, nowColors);
		//engage custom frame buffer


	}

	glfwTerminate();
	return 0;
}