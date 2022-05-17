#include "Window.h"

int screenWidth;
int screenHeight;

GLID frameBufferID;
GLID frameTextureID;
GLID renderBufferID;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);


Window::Window(int width, int height, std::string header)
	:title(header)
{
	screenWidth = width;
	screenHeight = height;
	frameBufferID = 0;
	frameTextureID = 0;
	renderBufferID = 0;

	cameraSpeed = 2.0f;
}

bool Window::initialize()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	auto monitor = glfwGetPrimaryMonitor();

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	//glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	//glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	//glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	//glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);



	glWindow = glfwCreateWindow(screenWidth, screenHeight, title.c_str(), NULL, NULL);
	//glWindow = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, NULL);

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

	/*if (glfwGetKey(glWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glWindow, true);

	
	
	if (glfwGetKey(glWindow, GLFW_KEY_COMMA) == GLFW_PRESS)
		paused = true;
	if (glfwGetKey(glWindow, GLFW_KEY_PERIOD) == GLFW_PRESS)
		paused = false;
	*/

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
		strafe.x -= camVel;

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
		*/
}

void Window::render(std::vector<Model> models, Shader objectShader, std::vector<glm::vec4> colors)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for(int i =0; i < models.size(); i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//engage normal shader
		objectShader.use();
		glDepthMask(GL_TRUE);
		{
			float nearPlaneDistance = 0.1f;
			glm::mat4 view;
			view = glm::scale(glm::translate(glm::mat4{1},glm::vec3(-1.0f,-2.f/3.f,0.0f)),glm::vec3(2.0f/40.0f,2.0f/3.0f,1.0f));
			GLID viewLoc = glGetUniformLocation(objectShader.ID, "worldToView");
			GLID colorLoc = glGetUniformLocation(objectShader.ID, "color");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniform4fv(colorLoc, 1, glm::value_ptr(colors.at(i)));

			models.at(i).Draw(objectShader);
		}
	}
	//event + buffer swap
	glfwPollEvents();
	glfwSwapBuffers(glWindow);
}
