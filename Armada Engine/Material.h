#pragma once
#include "Shader.h"

class Material {
	Shader const * const shader;
public:
	void setViewTransform(glm::mat4 view, glm::mat4 proj) const;
	Material(Shader const * const);
	Material() = delete;
	Material(const Material&) = delete;
	virtual void use() const = 0;
	Shader const* const getShader() const;
};