#pragma once
#include "Material.h"
#include "SolidLight.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>

class MaterialLightShadow :
    public Material
{
    class MaterialLight :
        public Material
    {
    public:
        MaterialLight(Shader const* const);
        void use() const override;
        void setLightParameters(SolidLight, float npd);
    };

    MaterialLight lightMat;
public:
    MaterialLightShadow(Shader const* const shadowStamp, Shader const* const lightBlend);
    void use() const override;
    void useNext() const;
    void setViewTransformNext(glm::mat4 view, glm::mat4 proj) const;
    void setLightParameters(SolidLight, float npd);
    void setLightParametersNext(SolidLight, float npd);
    Shader const * const getShaderNext() const;
};

