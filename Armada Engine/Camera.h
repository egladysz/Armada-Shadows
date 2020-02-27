#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
class Camera
{
private:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;

public:
	Camera();
	~Camera();
	void setPosition(glm::vec3 pos);
	void setForward(glm::vec3 front);
	void setUp(glm::vec3 upwards);
	void translateWorld(glm::vec3 dPos);
	void translateLocal(glm::vec3 strafe);
	
	glm::vec3 getPosition() const;
	glm::vec3 getForward() const;
	glm::vec3 getRight() const;
	glm::vec3 getUp() const;
	glm::mat4 getView() const;
};

