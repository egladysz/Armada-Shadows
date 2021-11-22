#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderElement.h"

class Shader
{
public:

	unsigned int ID;

	Shader();

	void use() const;

	void addElement(ShaderElement);
	void prime();

	template <typename T>
	void setUniform(const std::string& name, T value) const;
	template <>
	void setUniform(const std::string& name, float value) const;
	template <>
	void setUniform(const std::string& name, glm::vec3 value) const;
	template <>
	void setUniform(const std::string& name, glm::vec4 value) const;
	template <>
	void setUniform(const std::string& name, glm::mat4 value) const;
	~Shader();
};

#include "ShaderTemplate.h"