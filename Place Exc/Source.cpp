#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "Window.h"
#include "../Armada Engine/Shader.h"
#include "../Armada Engine/Mesh.h"
#include "../Armada Engine/Model.h"
#include "../Armada Engine/SolidLight.h"
#include "../Armada Engine/LightScene.h"
#include "../Armada Engine/MeshReaderObj.h"
#include <chrono>
#include <map>
unsigned int screenWidth = 1600;
unsigned int screenHeight = 120;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool paused = false;




int main()
{
	Window window(screenWidth, screenHeight, "Exception.EXE Place");
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

	std::map<char, glm::vec4> colorCoded;

	//index code
	colorCoded.emplace(0, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));		//black
	colorCoded.emplace(1, glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));		//maroo
	colorCoded.emplace(2, glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));		//green
	colorCoded.emplace(3, glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));		//olive
	colorCoded.emplace(4, glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));		//navy
	colorCoded.emplace(5, glm::vec4(0.5f, 0.0f, 0.5f, 1.0f));		//purpl
	colorCoded.emplace(6, glm::vec4(0.0f, 0.5f, 0.5f, 1.0f));		//teal
	colorCoded.emplace(7, glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));	//silve
	colorCoded.emplace(8, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));		//gray
	colorCoded.emplace(9, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));		//red
	colorCoded.emplace(0xA, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));		//lime
	colorCoded.emplace(0xB, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));		//yello
	colorCoded.emplace(0xC, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));		//blue
	colorCoded.emplace(0xD, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));		//fuchs
	colorCoded.emplace(0xE, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));		//aqua
	colorCoded.emplace(0xF, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));		//white

	//cellular automata code
	colorCoded.emplace('f', glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));		//OFF, white
	colorCoded.emplace('t', glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));		//ON, black
	auto MataFalse = colorCoded['f'];
	auto MataTrue = colorCoded['t'];

	//color name code
	colorCoded.emplace('b', glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));		//black
	colorCoded.emplace('r', glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));		//maroon
	colorCoded.emplace('g', glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));		//green
	colorCoded.emplace('y', glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));		//olive
	colorCoded.emplace('u', glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));		//navy
	colorCoded.emplace('p', glm::vec4(0.5f, 0.0f, 0.5f, 1.0f));		//purple
	colorCoded.emplace('c', glm::vec4(0.0f, 0.5f, 0.5f, 1.0f));		//teal
	colorCoded.emplace('w', glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));	//silver
	colorCoded.emplace('B', glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));		//gray
	colorCoded.emplace('R', glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));		//red
	colorCoded.emplace('G', glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));		//lime
	colorCoded.emplace('Y', glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));		//yellow
	colorCoded.emplace('U', glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));		//blue
	colorCoded.emplace('P', glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));		//fuchsia
	colorCoded.emplace('C', glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));		//aqua
	colorCoded.emplace('W', glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));		//white



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



	for (int i = 0; i < 40; i++)
	{
		Model sq;
		sq.mesh = &square;
		sq.transform = glm::scale(glm::translate(glm::mat4{ 1 }, glm::vec3(0.5f + i, 2.f, 0.f)), glm::vec3(0.5f, 0.5f, 1.0f));
		models.push_back(sq);
	}

	for (int j = 0; j < 40; j++)
	{
		Model sq;
		sq.mesh = &square;
		sq.transform = glm::scale(glm::translate(glm::mat4{ 1 }, glm::vec3(0.5f + j, 1.f, 0.f)), glm::vec3(0.5f, 0.5f, 1.0f));
		models.push_back(sq);
	}
	for (int k = 0; k < 40; k++)
	{
		Model sq;
		sq.mesh = &square;
		sq.transform = glm::scale(glm::translate(glm::mat4{ 1 }, glm::vec3(0.5f + k, 0.f, 0.f)), glm::vec3(0.5f, 0.5f, 1.0f));
		models.push_back(sq);
	}
	std::vector<glm::vec4> grid{}; // the vector of colors for the models
	for (auto square : models)
		grid.push_back(colorCoded[0]);

	const auto epochStart = std::chrono::time_point<std::chrono::system_clock>{};
	const auto timerStart = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - epochStart).count();




	//update from file color data
	auto updateFileColors = [&grid, &colorCoded]() {
		std::ifstream input;
		input.open("./placedata.txt");

		if (input.is_open()) {
			for (int i = 0; i < grid.size(); i++) {
				grid[i] = colorCoded[0];
				char c = static_cast<char>(input.get());
				if (input.eof())
					continue;
				if (c == '\n') {
					i--;
					continue;
				}
				if (colorCoded.find(c) != colorCoded.end())
					grid[i] = colorCoded[c];
			}
			input.close();
		}
	};

	//update via Cellular Automata
	auto setupAutomata = [&grid, &colorCoded]() {
		std::string startCode = "ftffftftfttttffffttfffttfftftttfftfttfff";
		for (int i = 0; i < 80; i++)
			grid[i] = colorCoded['f'];

		for (int i = 80; i < 120; i++)
			grid[i] = colorCoded[startCode[i - 80]];
	};



	auto updateAutomata = [&grid, &colorCoded, MataTrue, MataFalse](int rule) {
		bool rules[8];
		for (int i = 0; i < 8; i++)
			rules[i] = (rule >> i) & 1;

		for (int i = 0; i < 80; i++) {
			grid[i] = grid[i + 40];
		}
		//01011001
		//110,100,011,000
		grid[80] = grid[41];
		grid[119] = grid[78];

		for (int i = 80; i < 120; i++)
		{
			bool a = grid[i - 41] == MataTrue;
			bool b = grid[i - 40] == MataTrue;
			bool c = grid[i - 39] == MataTrue;
			if (i == 80)
				a = grid[79] == MataTrue;
			if (i == 119)
				c = grid[40] == MataTrue;

			int ruleMask = 0;
			if (a) ruleMask += 4;
			if (b) ruleMask += 2;
			if (c) ruleMask += 1;

			grid[i] = (rules[ruleMask]) ? MataTrue : MataFalse;
		}
	};

	auto updateScroll = [&grid]() {
		auto left1 = grid[0];
		auto left2 = grid[40];
		auto left3 = grid[80];
		for (int i = 0; i < 39; i++) {
			grid[i] = grid[i + 1];
			grid[40 + i] = grid[i + 41];
			grid[80 + i] = grid[i + 81];
		}
		grid[39] = left1;
		grid[79] = left2;
		grid[119] = left3;
	};

	int currentMode = 1;
	int nextMode = 0;
	bool paused = false;
	while (!glfwWindowShouldClose(window.glWindow))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input

		static long long lastMoment;
		static float timeValue = static_cast<float>(glfwGetTime());
		timeValue += deltaTime;
		auto now = std::chrono::system_clock::now();
		auto epochTime = std::chrono::duration_cast<std::chrono::seconds>(now - epochStart).count();
		window.processInput(deltaTime, paused, nextMode);
		if (lastMoment != epochTime) // its a new second
		{
			if (nextMode == 0) {
				switch (currentMode) {
				case 1:
					updateFileColors();
					break;
				case 2:
					updateScroll();
					break;
				case 3:
					updateAutomata(89);
					break;
				default:
					break;
				}
			}
			else {
				switch (nextMode) {
				case 1:
					updateFileColors();
					break;
				case 2:
					updateFileColors();
					break;
				case 3:
					setupAutomata();
					break;
				default:
					break;
				}
				currentMode = nextMode;
				nextMode = 0;
			}
			//EMERGENCY MODIFICATION: Only render once a second
			window.render(models, simpleShader, grid);
		}
		lastMoment = epochTime;


		//render

		// 0. copy our vertices array in a buffer for OpenGL to use
		// 1. then set the vertex attributes pointers
		// 2. use our shader program when we want to render an object

		//window.render(models, simpleShader, grid);
		//engage custom frame buffer


	}

	glfwTerminate();
	return 0;
}