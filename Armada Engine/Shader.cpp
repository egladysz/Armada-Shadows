#include "Shader.h"

Shader::Shader()
	:ID{0}
{
	ID = glCreateProgram();
}

void Shader::prime()
{
	int successCode;
	char log[512];

	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &successCode);
	if (!successCode)
	{
		glGetProgramInfoLog(ID, 512, NULL, log);
		std::cout << "ERROR: Shader Linking Failed\n" << log << std::endl;
		ID = 0;
	}
}


void Shader::use() const
{
	glUseProgram(ID);
}

void Shader::addElement(ShaderElement se)
{
	glAttachShader(ID, se.getID());
}

Shader::~Shader()
{
}
