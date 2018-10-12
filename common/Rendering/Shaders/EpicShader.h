#pragma once

#ifndef __EPIC_SHADER_SHADER__
#define __EPIC_SHADER_SHADER__

#include "common/Rendering/Shaders/ShaderProgram.h"
#include <functional>

/*! \brief The CPU Interface to the Epic shader (either or: vert or frag version or the textured version).
 *
 *  Note that this shader class can also probably be used for any minor variations on the Blinn-Phong shader.
 */
class EpicShader: public ShaderProgram
{
public:
    /*! \brief Construcs the Blinn-Phong shader object.
     *  \param inputShaders Look at ShaderProgram::ShaderProgram() for details.
     *  \param lightingStage If subroutines are not disabled, this is the shader object (vertex or fragment) that contains the subroutine uniform variable.
     */
    EpicShader(const std::unordered_map<GLenum, std::string>& inputShaders);

    /*! \brief Deconstructor.
     */
    virtual ~EpicShader();

    /*! \copydoc ShaderProgram::SetupShaderLighting(const class Light* light) const;
     *  
     *  Makes sure the proper lighting function is called (i.e. for point light, directional, etc.). Additionally, passes in the light properties to the
     *  shader as well as any light attenuation properties.
     */
    virtual void SetupShaderLighting(const class Light* light) const; 

    /*! \copydoc ShaderProgram::SetupShaderMaterials() const
     *
     *  Makes sure that the uniform block object is bound and handles binding the diffuse and specular textures if they exist.
     */
    virtual void SetupShaderMaterials() const;

    /*! \copydoc ShaderProgram::SetupShaderCamera(const class Camera*) const
     *
     *  Copies the camera position to the shader.
     */
    virtual void SetupShaderCamera(const class Camera* camera) const;

    virtual void SetRoughness(float inRoughness);
    virtual void SetMetallic(float inRetallic);
    virtual void SetSpecular(glm::vec4 inSpecular);

    virtual void LoadMaterialFromAssimp(std::shared_ptr<struct aiMaterial> assimpMaterial);
protected:
    // Material Parameters
    virtual void UpdateMaterialBlock() const;

    float roughness;
    glm::vec4 specular;
    float metallic;

    // Material Bindings into the Shader
    static std::array<const char*, 3> MATERIAL_PROPERTY_NAMES;
    static const int MATERIAL_BINDING_POINT;
    GLuint materialBlockLocation;
    GLint materialBlockSize;
    std::array<GLuint, 3> materialIndices;
    std::array<GLint, 3> materialOffsets;
    GLuint materialBuffer;
    std::vector<GLubyte> materialStorage;

    // Attenuation Uniform Handling
    virtual void UpdateAttenuationUniforms(const class Light* light) const;
};


#endif
