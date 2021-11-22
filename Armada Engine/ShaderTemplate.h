#pragma once
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

template<typename T>
inline void Shader::setUniform(const std::string& name, T value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

template<>
inline void Shader::setUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), (float)value);
}

template<>
inline void Shader::setUniform(const std::string& name, glm::vec3 value) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);

}

template<>
inline void Shader::setUniform(const std::string& name, glm::vec4 value) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

template<>
inline void Shader::setUniform(const std::string& name, glm::mat4 value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}