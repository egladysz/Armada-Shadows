#include "Window.h"

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "SolidLight.h"
#include "LightScene.h"
#include "MeshReaderObj.h"
unsigned int screenWidth = 800;
unsigned int screenHeight = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;





bool paused = false;




int main()
{
	Window window(screenWidth, screenHeight, "Armada Engine - Demo Candidate 2");
	//GLFW setup
	if (!window.initialize())
	{
		return -1;
	}




	ShaderElement simpleVS("VS/Simple.vs", GL_VERTEX_SHADER);
	ShaderElement simpleFS("FS/Simple.fs", GL_FRAGMENT_SHADER);

	ShaderElement shadowVS("VS/Shadow.vs", GL_VERTEX_SHADER);
	ShaderElement shadowGS("GS/ShadowMaker.gs", GL_GEOMETRY_SHADER);
	ShaderElement shadowFS("FS/Shadow.fs", GL_FRAGMENT_SHADER);

	ShaderElement samplerVS("VS/Sampler.vs", GL_VERTEX_SHADER);
	ShaderElement samplerFS("FS/Sampler.fs", GL_FRAGMENT_SHADER);

	ShaderElement lightVS("VS/Light.vs", GL_VERTEX_SHADER);
	ShaderElement lightFS("FS/Light.fs", GL_FRAGMENT_SHADER);

	Shader simpleShader;
	Shader shadowStamp;
	Shader lightStamp;
	Shader lightBlender;

	simpleShader.addElement(simpleVS);
	simpleShader.addElement(simpleFS);
	simpleShader.prime();


	shadowStamp.addElement(shadowVS);
	shadowStamp.addElement(shadowGS);
	shadowStamp.addElement(shadowFS);
	shadowStamp.prime();

	lightStamp.addElement(samplerVS);
	lightStamp.addElement(samplerFS);
	lightStamp.prime();

	lightBlender.addElement(lightVS);
	lightBlender.addElement(lightFS);
	lightBlender.prime();


	MeshReaderObj meshReader;
	
	auto cube = meshReader.loadMesh("Mesh/cube.obj");
	auto humen = meshReader.loadMesh("Mesh/humen.obj");
	auto house = meshReader.loadMesh("Mesh/remhaus.obj");

	std::vector<LightScene> allScenes;


	//Scene 0: a tiny cube that tracks the scene 1 light


	//Scene 1: One light, 2 static objects
	std::vector<Model*> m1;
	std::vector<SolidLight*> l1;
	Model s1c1;
	Model s1c2;
	SolidLight s1l1;
	{

		s1l1.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		s1l1.lightRadius = 40;
		s1l1.shadowLength = 40;
		s1l1.position = glm::vec3(0.0f, 0.0f, 0.0f);
		s1l1.on = true;
		l1.push_back(&s1l1);

		s1c1.mesh = humen.get();
		s1c1.transform = glm::translate(glm::mat4{ 1 }, glm::vec3(0.0f, -20.0f, 0.0f));
		
		s1c2.mesh = cube.get();
		//s1c2.transform = glm::scale(glm::rotate(glm::translate(glm::mat4{ 1 }, glm::vec3(0.0f, -2.0f, 0.0f)),glm::radians(45.f),glm::vec3(1.0f,0.5f,0.25f)),glm::vec3(2.0f,0.5f,1.0f));
		s1c2.transform = glm::scale(glm::translate(glm::mat4{ 1 },glm::vec3(0.0f,-70.0f,0.0f)),glm::vec3(100.0f,100.0f,100.0f));

		m1.push_back(&s1c1);
		m1.push_back(&s1c2);

		LightScene s1{ l1,m1,shadowStamp,lightBlender };
		allScenes.push_back(s1);
	}

	//Scene 2: One light, one orbiting object

	std::vector<Model*> m2;
	std::vector<SolidLight*> l2;
	Model s2c1;
	SolidLight s2l1;
	SolidLight s2l2;
	SolidLight s2l3;
	{
		s2l1.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
		s2l1.lightRadius = 5;
		s2l1.shadowLength = 200;
		s2l1.on = true;

		s2l2.color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		s2l2.lightRadius = 5;
		s2l2.shadowLength = 200;
		s2l2.on = true;
		s2l2.position.y = -3.0;

		s2l3.color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
		s2l3.lightRadius = 5;
		s2l3.shadowLength = 200;
		s2l3.on = true;
		s2l3.position.y = 3.0f;

		l2.push_back(&s2l1);
		l2.push_back(&s2l2);
		l2.push_back(&s2l3);
		s2c1.mesh = cube.get();
		m2.push_back(&s2c1);
		//transform time dependent
		LightScene s2{ l2,m2,shadowStamp,lightBlender };
		s2.local = glm::mat4{ 1 };// glm::translate(glm::mat4{ 1 }, glm::vec3(15.0f, 0.0f, 0.0f));
		allScenes.push_back(s2);
	}

	//Scene 3: Several lights, No objects
	std::vector<Model*> m3;
	std::vector<SolidLight*> l3;
	SolidLight s3l1;
	SolidLight s3l2;
	SolidLight s3l3;
	{
		s3l1.color = glm::vec4(1.0f, 0.0f, 0.0f, 0.2f);
		s3l1.lightRadius = 5;
		s3l1.on = true;

		s3l2.color = glm::vec4(.5f, 0.5f, 0.0f, 0.5f);
		s3l2.lightRadius = 4;
		s3l2.on = true;

		s3l3.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.6f);
		s3l3.lightRadius = 1;
		s3l3.on = true;

		l3.push_back(&s3l1);
		l3.push_back(&s3l2);
		l3.push_back(&s3l3);

		LightScene s3{ l3,m3,shadowStamp,lightBlender };
		s3.local = glm::mat4{ 1 };// glm::translate(glm::mat4{ 1 }, glm::vec3(30.0f, 0.0f, 0.0f));
		allScenes.push_back(s3);
	}

	//Scene 4: One Light of varying intensity, one wall
	std::vector<Model*> m4;
	std::vector<SolidLight*> l4;
	SolidLight s4l1;
	Model s4c1;
	{
		s4l1.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		s4l1.lightRadius = 40;
		s4l1.shadowLength = 20;
		s4l1.on = true;

		l4.push_back(&s4l1);

		s4c1.mesh = cube.get();
		s4c1.transform = glm::translate(glm::scale(glm::mat4{ 1 }, glm::vec3(1.0f, 20.0f, 1.0f)), glm::vec3(-4.0f, 0.0f, 0.0f));

		m4.push_back(&s4c1);
		LightScene s4{ l4,m4,shadowStamp,lightBlender };
		s4.local = glm::mat4{ 1 };// glm::translate(glm::mat4{ 1 }, glm::vec3(40.0f, 0.0f, 0.0f));
		allScenes.push_back(s4);
	}


	//Scene 5: A few lights. Numerous objects. All mayhem
	std::vector<Model*> m5;
	std::vector<SolidLight*> l5;
	SolidLight s5l[3];
	Model s5c[49];
	{
		s5l[0].color = glm::vec4(1.0f, 0.0f, 0.0f, 0.1f);
		s5l[1].color = glm::vec4(0.0f, 1.0f, 0.0f, 0.5f);
		s5l[2].color = glm::vec4(0.0f, 0.0f, 1.0f, 0.9f);

		s5l[0].lightRadius = 20;
		s5l[1].lightRadius = 35;
		s5l[2].lightRadius = 50;

		s5l[0].shadowLength = 50;
		s5l[1].shadowLength = 50;
		s5l[2].shadowLength = 50;



		s5l[0].on = true;
		s5l[1].on = true;
		s5l[2].on = true;

		l5.push_back(&s5l[0]);
		l5.push_back(&s5l[1]);
		l5.push_back(&s5l[2]);

		for (int i = 0; i < 49; i++)
		{
			s5c[i].mesh = cube.get();

			m5.push_back(&s5c[i]);
		}
		LightScene s5{ l5,m5,shadowStamp,lightBlender };
		s5.local = glm::mat4{1};// glm::translate(glm::mat4{ 1 }, glm::vec3(105.0f, 0.0f, 0.0f));
		allScenes.push_back(s5);
	}
		


	window.setCamera(	glm::vec3(0.0f, 0.0f, 10.0f),
						glm::vec3(0.0f, 0.0f, -1.0f),
						glm::vec3(0.0f,1.0f,0.0f));






	while (!glfwWindowShouldClose(window.glWindow))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input
		static int scene = 1;
		int lastScene = scene;
		window.processInput(deltaTime,paused,scene);



		static float timeValue = glfwGetTime();
		if (!paused)
			timeValue += deltaTime;

		if (lastScene != scene)
		{
			timeValue = 0;
		}

		//Scene 1. No time component
		s1l1.position.z = 0;// 5 * cos(glm::radians(30 * timeValue));

		//Scene 2. Orbiting cube
		s2c1.transform = glm::translate(glm::rotate(glm::mat4{1}, glm::radians(90 * timeValue), glm::vec3(0.0f, 0.0f, 1.0f)),glm::vec3(2.0f,0.0f,0.0f));
		s2l1.position.x = 5*cos(glm::radians(30 * timeValue));
		//Scene 3. No time componentd

		//Scene 4. Flickering Light
		s4l1.color.a = 0.5f+0.5f*sin(timeValue*8);
		s4l1.color.g = cos(3*timeValue);
		s4l1.color.b = cos(5 * timeValue) + 1.5f;

		//Scene 5. Planerary Shenanigan
		for (int i = 0; i < 49; i++)
		{
			glm::mat4 transform{ 1 };
			float indexRotation = (49 - i)*(49 - i)* (49 - i) /2500.0f;
			transform = glm::rotate(transform, glm::radians(indexRotation*(timeValue+1000.0f)), glm::vec3(0.0f,0.0f,1.0f));
			transform = glm::translate(transform, glm::vec3(i+4, 0.0f, 0.0f));
			s5c[i].transform = transform;
		}

		//render
		


		// 0. copy our vertices array in a buffer for OpenGL to use
		// 1. then set the vertex attributes pointers
		// 2. use our shader program when we want to render an object


		auto currentScene = std::vector<LightScene>();
		currentScene.push_back(allScenes.at(scene - 1));
		window.render(currentScene, simpleShader);
		currentScene.pop_back();
		//engage custom frame buffer


	}

	glfwTerminate();
	return 0;
}