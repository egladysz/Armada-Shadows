#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::setPosition(glm::vec3 pos)
{
	position = pos;
}

void Camera::setForward(glm::vec3 front)
{
	forward = glm::normalize(front);
}

void Camera::setUp(glm::vec3 upwards)
{
	up = glm::normalize(upwards);
}

void Camera::translateWorld(glm::vec3 dPos)
{
	position += dPos;
}

void Camera::translateLocal(glm::vec3 strafe)
{
	position += forward*strafe.x + up*strafe.y + getRight()*strafe.z;
}

glm::vec3 Camera::getPosition() const
{
	return position;
}

glm::vec3 Camera::getForward() const
{
	return forward;
}

glm::vec3 Camera::getRight() const
{
	return glm::normalize(glm::cross(forward,up));
}

glm::vec3 Camera::getUp() const
{
	return up;
}

glm::mat4 Camera::getView() const
{
	return glm::lookAt(position, position + forward, up);
}
