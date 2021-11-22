#include "MaterialBasicColor.h"

MaterialBasicColor::MaterialBasicColor(Shader const* const shader, glm::vec4 color)
	:Material{ shader }, rgba{ color } {}

void MaterialBasicColor::use() const
{
	getShader()->use();
	getShader()->setUniform("color", rgba);
}
