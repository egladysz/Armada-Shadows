#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Terminology.h"

class ShaderElement
{
private:
	GLID shaderID;
public:
	ShaderElement(const GLchar* path, GLenum shaderType);
	ShaderElement(const std::string text, GLenum shaderType, bool is);
	~ShaderElement();

	GLID getID();
};

