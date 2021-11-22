#pragma once
#include "Material.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>

class MaterialBasicColor :
    public Material
{
    glm::vec4 rgba;
public:
    MaterialBasicColor(Shader const* const, glm::vec4);
    void use() const override;
};

