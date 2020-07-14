#include "Window.h"
#include "Camera.h"

int screenWidth;
int screenHeight;

GLID frameBufferID;
GLID frameTextureID;
GLID renderBufferID;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
Camera cam;


Window::Window(int width, int height, std::string header)
	:title(header)
{
	screenWidth = width;
	screenHeight = height;
	frameBufferID = 0;
	frameTextureID = 0;
	renderBufferID = 0;

	cameraSpeed = 20.0f;
}

bool Window::initialize()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(screenWidth, screenHeight, title.c_str(), NULL, NULL);
	if (glWindow == NULL)
	{
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(glWindow);
	glfwSetFramebufferSizeCallback(glWindow, framebuffer_size_callback);

	//GLAD setup
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "GLAD initialization failed" << std::endl;
		return false;
	}

	glViewport(0, 0, screenWidth, screenHeight);
	framebuffer_size_callback(glWindow, screenWidth, screenHeight);
	return true;
}

void Window::setCamera(glm::vec3 pos, glm::vec3 forward, glm::vec3 up)
{
	cam.setPosition(pos);
	cam.setForward(forward);
	cam.setUp(up);
}

Window::~Window()
{
	if (frameBufferID)
	{
		glDeleteFramebuffers(1, &frameBufferID);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	screenWidth = width;
	screenHeight = height;

	if (frameBufferID)
	{
		glDeleteFramebuffers(1, &frameBufferID);
	}
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	if (frameTextureID)
	{
		glDeleteTextures(1, &frameTextureID);
	}
	glGenTextures(1, &frameTextureID);
	glBindTexture(GL_TEXTURE_2D, frameTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//attach
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTextureID, 0);

	if (renderBufferID)
	{
		glDeleteRenderbuffers(1, &renderBufferID);
	}
	glGenRenderbuffers(1, &renderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attach
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);
}

void Window::processInput(float deltaTime, bool& paused, int& scene)
{

	if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glWindow, true);

	
	
	if (glfwGetKey(glWindow, GLFW_KEY_COMMA) == GLFW_PRESS)
		paused = true;
	if (glfwGetKey(glWindow, GLFW_KEY_PERIOD) == GLFW_PRESS)
		paused = false;
		
	int lastScene = scene;

	if (glfwGetKey(glWindow, GLFW_KEY_1) == GLFW_PRESS)
		scene = 1;
	if (glfwGetKey(glWindow, GLFW_KEY_2) == GLFW_PRESS)
		scene = 2;
	if (glfwGetKey(glWindow, GLFW_KEY_3) == GLFW_PRESS)
		scene = 3;
	if (glfwGetKey(glWindow, GLFW_KEY_4) == GLFW_PRESS)
		scene = 4;
	if (glfwGetKey(glWindow, GLFW_KEY_5) == GLFW_PRESS)
		scene = 5;
	if (glfwGetKey(glWindow, GLFW_KEY_6) == GLFW_PRESS)
		scene = 6;


	/*if (glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS)
		strafe.x += camVel;
	if (glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS)
		strafe.x -= camVel;*/

	glm::vec3 strafe{ 0.0f };
	float camVel = cameraSpeed * deltaTime;
	float dx = 0.0f;
	if (glfwGetKey(glWindow, GLFW_KEY_Q) == GLFW_PRESS)
		dx += camVel;
	if (glfwGetKey(glWindow, GLFW_KEY_E) == GLFW_PRESS)
		dx -= camVel;
	if (glfwGetKey(glWindow, GLFW_KEY_W) == GLFW_PRESS)
		strafe.y += camVel;
	if (glfwGetKey(glWindow, GLFW_KEY_S) == GLFW_PRESS)
		strafe.y -= camVel;
	if (glfwGetKey(glWindow, GLFW_KEY_A) == GLFW_PRESS)
		strafe.z -= camVel;
	if (glfwGetKey(glWindow, GLFW_KEY_D) == GLFW_PRESS)
		strafe.z += camVel;
	if (glfwGetKey(glWindow, GLFW_KEY_R) == GLFW_PRESS)
	{
		cam.setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
		cam.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	float camDistance = glm::max(1.0f,glm::length(cam.getPosition()) + dx);
	if (lastScene == scene)
	{
		cam.translateLocal(strafe*camDistance/5.0f);
		
	}
	else
	{
		cam.setPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	}
	cam.setForward(-1.0f*cam.getPosition());
	cam.setUp(glm::cross(cam.getRight(), cam.getForward()));
	cam.setPosition(glm::normalize(cam.getPosition()) * (camDistance));
}

void Window::render(std::vector<LightScene> scenes, Shader objectShader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (LightScene s : scenes)
	{
		s.Render(cam.getForward(), cam.getPosition(), cam.getView(), frameBufferID, frameTextureID, screenWidth, screenHeight);
	}

	for (LightScene s : scenes)
	{
		s.RenderObjects(objectShader,cam.getView(), screenWidth, screenHeight);
	}
	//event + buffer swap
	glfwPollEvents();
	glfwSwapBuffers(glWindow);
}
