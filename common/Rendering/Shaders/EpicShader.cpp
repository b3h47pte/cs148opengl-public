#include "common/Rendering/Shaders/EpicShader.h"
#include "common/Rendering/Textures/Texture2D.h"
#include "common/Scene/Light/Light.h"
#include "common/Scene/Light/LightProperties.h"
#include "common/Scene/Camera/Camera.h"
#include "common/Utility/Texture/TextureLoader.h"
#include "assimp/material.h"

std::array<const char*, 3> EpicShader::MATERIAL_PROPERTY_NAMES = {
    "InputMaterial.matRoughness", 
    "InputMaterial.matSpecular", 
    "InputMaterial.matMetallic", 
};
const int EpicShader::MATERIAL_BINDING_POINT = 0;

EpicShader::EpicShader(const std::unordered_map<GLenum, std::string>& inputShaders):
    ShaderProgram(inputShaders), roughness(0.5f), specular(glm::vec3(0.f), 1.f), metallic(0.5f),
    materialBlockLocation(0), materialBlockSize(0), materialBuffer(0)
{
    if (!shaderProgram) {
        return;
    }

    SetupUniformBlock<3>("InputMaterial", MATERIAL_PROPERTY_NAMES, materialIndices, materialOffsets, materialStorage, materialBlockLocation, materialBlockSize, materialBuffer);
    UpdateMaterialBlock();
}

EpicShader::~EpicShader()
{
    OGL_CALL(glDeleteBuffers(1, &materialBuffer));
}

void EpicShader::SetupShaderLighting(const Light* light) const
{
    if (!light) {
        SetShaderUniform("lightingType", static_cast<int>(Light::LightType::GLOBAL));
    } else {
        const LightProperties* lightProperty = light->GetPropertiesRaw();

        // Select proper lighting subroutine based on the light's type.
        switch(light->GetLightType()) {
        case Light::LightType::POINT:
            SetShaderUniform("lightingType", static_cast<int>(light->GetLightType()));
            break;
        case Light::LightType::DIRECTIONAL:
            SetShaderUniform("lightingType", static_cast<int>(light->GetLightType()));
            break;
        case Light::LightType::HEMISPHERE:
            SetShaderUniform("lightingType", static_cast<int>(light->GetLightType()));
            break;
        case Light::LightType::SPOT:
            {
                const SpotlightProperties* spotProperties = dynamic_cast<const SpotlightProperties*>(lightProperty);
                SetShaderUniform("lightingType", static_cast<int>(light->GetLightType()));
                SetShaderUniform("genericLight.spotInnerConeAngleDegrees", spotProperties->innerConeAngleDegrees);
                SetShaderUniform("genericLight.spotOuterConeAngleDegrees", spotProperties->outerConeAngleDegrees);
                break;
            }
        default:
            SetShaderUniform("lightingType", static_cast<int>(Light::LightType::GLOBAL));
            break;
        }

        // Get the light's properties and pass it into the shader.
        SetShaderUniform("genericLight.diffuseColor", lightProperty->diffuseColor);
        SetShaderUniform("genericLight.specularColor", lightProperty->specularColor);
        SetShaderUniform("genericLight.directionalLightDir", light->GetForwardDirection());
        light->SetupShaderUniforms(this);
    }
    UpdateAttenuationUniforms(light);
}

void EpicShader::UpdateMaterialBlock() const
{
    StartUseShader();

    if (materialStorage.data()) {
        memcpy((void*)(materialStorage.data() + materialOffsets[0]), &roughness, sizeof(float));
        memcpy((void*)(materialStorage.data() + materialOffsets[1]), glm::value_ptr(specular), sizeof(glm::vec4));
        memcpy((void*)(materialStorage.data() + materialOffsets[2]), &metallic, sizeof(float));

        if (materialBuffer && materialBlockLocation != GL_INVALID_INDEX) {
            OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, materialBuffer));
            OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, materialBlockSize, materialStorage.data(), GL_STATIC_DRAW));
            OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, MATERIAL_BINDING_POINT, materialBuffer));
            OGL_CALL(glUniformBlockBinding(shaderProgram, materialBlockLocation, MATERIAL_BINDING_POINT));
            OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
        }
    }

    StopUseShader();
}

void EpicShader::UpdateAttenuationUniforms(const Light* light) const
{
    float constant = 1.f, linear = 0.f, quadratic = 0.f;
    if (light) {
        light->GetAttenuation(constant, linear, quadratic);
    }

    SetShaderUniform("constantAttenuation", constant);
    SetShaderUniform("linearAttenuation", linear);
    SetShaderUniform("quadraticAttenuation", quadratic);
}

void EpicShader::SetupShaderMaterials() const
{
    // Need to make sure the material buffer is bound.
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, MATERIAL_BINDING_POINT, materialBuffer));
}

void EpicShader::SetupShaderCamera(const class Camera* camera) const
{
    SetShaderUniform("cameraPosition", camera->GetPosition());
}

void EpicShader::SetRoughness(float inRoughness)
{
    roughness = inRoughness;
    UpdateMaterialBlock();
}

void EpicShader::SetMetallic(float inMetallic)
{
    metallic = inMetallic;
    UpdateMaterialBlock();
}

void EpicShader::SetSpecular(glm::vec4 inSpecular)
{ 
    specular = inSpecular; 
    UpdateMaterialBlock();
}

void EpicShader::LoadMaterialFromAssimp(std::shared_ptr<aiMaterial> assimpMaterial)
{
    std::cerr << "ERROR: LoadMaterialFromAssimp not supported for the EpicShader." << std::endl;
}
