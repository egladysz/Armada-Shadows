#include "Material.h"

void Material::setViewTransform(glm::mat4 view, glm::mat4 proj) const
{
	getShader()->setUniform("worldToView", view);
	getShader()->setUniform("viewToProjection", proj);
}

Material::Material(Shader const* const shader) :
	shader{ shader } {}
Shader const* const Material::getShader() const
{
	return shader;
}