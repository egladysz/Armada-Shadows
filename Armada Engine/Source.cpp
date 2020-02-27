#include "Window.h"

#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "SolidLight.h"
#include "LightScene.h"
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




	//testing
	Vertex vertices[24];
	{
		vertices[0].position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertices[1].position = glm::vec3(0.5f, -0.5f, 0.5f);
		vertices[2].position = glm::vec3(-0.5f, 0.5f, 0.5f);
		vertices[3].position = glm::vec3(-0.5f, -0.5f, 0.5f);
		vertices[4].position = glm::vec3(0.5f, 0.5f, -0.5f);
		vertices[5].position = glm::vec3(0.5f, -0.5f, -0.5f);
		vertices[6].position = glm::vec3(-0.5f, 0.5f, -0.5f);
		vertices[7].position = glm::vec3(-0.5f, -0.5f, -0.5f);

		vertices[8].position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertices[9].position = glm::vec3(0.5f, 0.5f, -0.5f);
		vertices[10].position = glm::vec3(-0.5f, 0.5f, 0.5f);
		vertices[11].position = glm::vec3(-0.5f, 0.5f, -0.5f);
		vertices[12].position = glm::vec3(0.5f, -0.5f, 0.5f);
		vertices[13].position = glm::vec3(0.5f, -0.5f, -0.5f);
		vertices[14].position = glm::vec3(-0.5f, -0.5f, 0.5f);
		vertices[15].position = glm::vec3(-0.5f, -0.5f, -0.5f);

		vertices[16].position = glm::vec3(0.5f, 0.5f, 0.5f);
		vertices[17].position = glm::vec3(0.5f, -0.5f, 0.5f);
		vertices[18].position = glm::vec3(0.5f, 0.5f, -0.5f);
		vertices[19].position = glm::vec3(0.5f, -0.5f, -0.5f);
		vertices[20].position = glm::vec3(-0.5f, 0.5f, 0.5f);
		vertices[21].position = glm::vec3(-0.5f, -0.5f, 0.5f);
		vertices[22].position = glm::vec3(-0.5f, 0.5f, -0.5f);
		vertices[23].position = glm::vec3(-0.5f, -0.5f, -0.5f);

		vertices[0].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices[1].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices[2].normal = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices[3].normal = glm::vec3(0.0f, 0.0f, 1.0f);

		vertices[4].normal = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices[5].normal = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices[6].normal = glm::vec3(0.0f, 0.0f, -1.0f);
		vertices[7].normal = glm::vec3(0.0f, 0.0f, -1.0f);


		vertices[8].normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices[9].normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices[10].normal = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices[11].normal = glm::vec3(0.0f, 1.0f, 0.0f);

		vertices[12].normal = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices[13].normal = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices[14].normal = glm::vec3(0.0f, -1.0f, 0.0f);
		vertices[15].normal = glm::vec3(0.0f, -1.0f, 0.0f);


		vertices[16].normal = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices[17].normal = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices[18].normal = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices[19].normal = glm::vec3(1.0f, 0.0f, 0.0f);

		vertices[20].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices[21].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices[22].normal = glm::vec3(-1.0f, 0.0f, 0.0f);
		vertices[23].normal = glm::vec3(-1.0f, 0.0f, 0.0f);




		vertices[0].uv = glm::vec2(1.0f, 1.0f);
		vertices[1].uv = glm::vec2(1.0f, 0.0f);
		vertices[2].uv = glm::vec2(0.0f, 1.0f);
		vertices[3].uv = glm::vec2(0.0f, 0.0f);
		vertices[4].uv = glm::vec2(1.0f, 1.0f);
		vertices[5].uv = glm::vec2(1.0f, 0.0f);
		vertices[6].uv = glm::vec2(0.0f, 1.0f);
		vertices[7].uv = glm::vec2(0.0f, 0.0f);

		vertices[8].uv = glm::vec2(1.0f, 1.0f);
		vertices[9].uv = glm::vec2(1.0f, 0.0f);
		vertices[10].uv = glm::vec2(0.0f, 1.0f);
		vertices[11].uv = glm::vec2(0.0f, 0.0f);
		vertices[12].uv = glm::vec2(1.0f, 1.0f);
		vertices[13].uv = glm::vec2(1.0f, 0.0f);
		vertices[14].uv = glm::vec2(0.0f, 1.0f);
		vertices[15].uv = glm::vec2(0.0f, 0.0f);

		vertices[16].uv = glm::vec2(1.0f, 1.0f);
		vertices[17].uv = glm::vec2(1.0f, 0.0f);
		vertices[18].uv = glm::vec2(0.0f, 1.0f);
		vertices[19].uv = glm::vec2(0.0f, 0.0f);
		vertices[20].uv = glm::vec2(1.0f, 1.0f);
		vertices[21].uv = glm::vec2(1.0f, 0.0f);
		vertices[22].uv = glm::vec2(0.0f, 1.0f);
		vertices[23].uv = glm::vec2(0.0f, 0.0f);
	}
	unsigned int indices[]{
		0,1,3,
		2,0,3,

		4,5,7,
		6,4,7,

		8,9,11,
		10,8,11,

		12,13,15,
		14,12,15,

		16,17,19,
		18,16,19,

		20,21,23,
		22,20,23

	};

	Mesh cube(vertices, 24, indices, 36);

	std::vector<LightScene> allScenes;


	//Scene 0: a tiny cube that tracks the scene 1 light


	//Scene 1: One light, 2 static objects
	std::vector<Model*> m1;
	std::vector<SolidLight*> l1;
	Model s1c1;
	Model s1c2;
	SolidLight s1l1;
	{

		s1l1.color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
		s1l1.lightRadius = 5;
		s1l1.shadowLength = 200;
		s1l1.on = true;
		l1.push_back(&s1l1);

		s1c1.mesh = &cube;
		s1c1.transform = glm::translate(glm::mat4{ 1 }, glm::vec3(2.0f, 1.0f, 0.0f));
		
		s1c2.mesh = &cube;
		s1c2.transform = glm::scale(glm::rotate(glm::translate(glm::mat4{ 1 }, glm::vec3(0.0f, -2.0f, 0.0f)),glm::radians(45.f),glm::vec3(1.0f,0.5f,0.25f)),glm::vec3(2.0f,0.5f,1.0f));

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
		s2c1.mesh = &cube;
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

		s4c1.mesh = &cube;
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
			s5c[i].mesh = &cube;

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
			transform = glm::rotate(transform, glm::radians((49 - i)*timeValue), glm::vec3(0.0f,0.0f,1.0f));
			transform = glm::translate(transform, glm::vec3(i+1, 0.0f, 0.0f));
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