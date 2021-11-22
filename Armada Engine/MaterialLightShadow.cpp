#include "MaterialLightShadow.h"


MaterialLightShadow::MaterialLightShadow(Shader const* const shadowStamp, Shader const* const lightBlend)
	:Material{ shadowStamp }, lightMat{ lightBlend } {}

void MaterialLightShadow::use() const
{
	getShader()->use();
	
}

void MaterialLightShadow::useNext() const
{
	lightMat.use();
}

void MaterialLightShadow::setViewTransformNext(glm::mat4 view, glm::mat4 proj) const
{
	lightMat.setViewTransform(view, proj);
}

void MaterialLightShadow::setLightParameters(SolidLight light, float npd)
{
	getShader()->setUniform("lightPos", light.position);
	getShader()->setUniform("lightColor", light.color);
	getShader()->setUniform("shadRad", light.shadowLength);
}

void MaterialLightShadow::setLightParametersNext(SolidLight sl, float npd)
{
	lightMat.setLightParameters(sl, npd);
}

Shader const * const MaterialLightShadow::getShaderNext() const
{
	return lightMat.getShader();
}

MaterialLightShadow::MaterialLight::MaterialLight(Shader const* const lightBlender)
	:Material{ lightBlender } {}

void MaterialLightShadow::MaterialLight::use() const
{
	getShader()->use();
	
}

void MaterialLightShadow::MaterialLight::setLightParameters(SolidLight light, float npd)
{
	getShader()->setUniform("lightPos", light.position);
	getShader()->setUniform("lightRad", light.lightRadius);
	getShader()->setUniform("nearPlaneDistance", npd);
}
