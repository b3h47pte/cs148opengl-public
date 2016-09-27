#include "assignment5/Assignment5.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include "common/Utility/Texture/TextureLoader.h"

Assignment5::Assignment5(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
}

std::unique_ptr<Application> Assignment5::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment5>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment5::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(60.f, 1280.f / 720.f);
}

glm::vec2 Assignment5::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment5::SetupScene()
{
    SetupExample1();
}

void Assignment5::SetupCamera()
{
    camera->SetPosition(glm::vec3(0.f, 0.f, 2.f));
}

void Assignment5::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    if (state != SDL_KEYDOWN) {
        return;
    }

    switch (key.sym) {
    case SDLK_1:
        if (!repeat && state == SDL_KEYDOWN) {
            SetupExample1();
        }
        break;
    case SDLK_2:
        if (!repeat && state == SDL_KEYDOWN) {
            SetupExample2();
        }
        break;
    case SDLK_UP:
        camera->Rotate(glm::vec3(camera->GetRightDirection()), 0.1f);
        break;
    case SDLK_DOWN:
        camera->Rotate(glm::vec3(camera->GetRightDirection()), -0.1f);
        break;
    case SDLK_RIGHT:
        camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), -0.1f);
        break;
    case SDLK_LEFT:
        camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.1f);
        break;
    case SDLK_w:
        camera->Translate(glm::vec3(camera->GetForwardDirection() * 0.3f));
        break;
    case SDLK_a:
        camera->Translate(glm::vec3(camera->GetRightDirection() * -0.3f));
        break;
    case SDLK_s:
        camera->Translate(glm::vec3(camera->GetForwardDirection() * -0.3f));
        break;
    case SDLK_d:
        camera->Translate(glm::vec3(camera->GetRightDirection() * 0.3f));
        break;
    case SDLK_SPACE:
        camera->Translate(glm::vec3(camera->GetUpDirection() * 0.3f));
        break;
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        camera->Translate(glm::vec3(camera->GetUpDirection() * -0.3f));
        break;
    default:
        Application::HandleInput(key, state, repeat, timestamp, deltaTime);
        break;
    }
}

void Assignment5::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
    std::static_pointer_cast<PerspectiveCamera>(camera)->SetAspectRatio(x / y);
}

void Assignment5::SetupExample1()
{
    scene->ClearScene();
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/blinnphong.frag" }
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.frag"}
    };
#endif
    std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
    shader->SetDiffuse(glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    shader->SetTexture(BlinnPhongShader::TextureSlots::DIFFUSE, TextureLoader::LoadTexture("brick/bricktexture.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::SPECULAR, TextureLoader::LoadTexture("brick/bricktexture.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::NORMAL, TextureLoader::LoadTexture("brick/bricktexture_norm.jpg"));
    shader->SetTexture(BlinnPhongShader::TextureSlots::DISPLACEMENT, TextureLoader::LoadTexture("brick/bricktexture_displacement.jpg"));
    shader->SetMaxDisplacement(0.1f);

    std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(glm::vec3(10.f, 10.f, 10.f));
    scene->AddLight(pointLight);

    std::vector<std::shared_ptr<RenderingObject>> sphereTemplate = MeshLoader::LoadMesh(shader, "sphere.obj");
    for (size_t i = 0; i < sphereTemplate.size(); ++i) {
        sphereTemplate[i]->ComputeTangentSpace();
    }

    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(sphereTemplate);
    sceneObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), PI / 4.f);
    scene->AddSceneObject(sceneObject);


    std::unordered_map<GLenum, std::string> cubeShaderSpec = {
        { GL_VERTEX_SHADER, "cubemap/cubemap.vert" },
        { GL_FRAGMENT_SHADER, "cubemap/cubemap.frag" }
    };
    std::shared_ptr<CubeMapTexture> skyboxTexture = TextureLoader::LoadCubeTexture("GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c05.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c01.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c00.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c02.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c03.bmp", 
        "GrandCanyon_C_YumaPoint/cubemap/grandcanyone_c04.bmp");
    std::shared_ptr<CubeMapShader> cubeShader = std::make_shared<CubeMapShader>(cubeShaderSpec, skyboxTexture);
    std::vector<std::shared_ptr<RenderingObject>> cubeTemplate = MeshLoader::LoadMesh(cubeShader, "cube.obj");
    for (size_t i = 0; i < cubeTemplate.size(); ++i) {
        cubeTemplate[i]->ReverseVertexOrder();
    }

    std::shared_ptr<class SceneObject> cubeObject = std::make_shared<SceneObject>(cubeTemplate);
    cubeObject->SetPosition(glm::vec3(0.f, 0.f, 2.f));
    scene->AddSceneObject(cubeObject);
}

void Assignment5::SetupExample2()
{
    scene->ClearScene();
#ifndef DISABLE_OPENGL_SUBROUTINES
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/blinnphong.frag" }
    };
#else
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.vert" },
        { GL_FRAGMENT_SHADER, "brdf/blinnphong/fragTexture/noSubroutine/blinnphong.frag" }
    };
#endif
    std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->diffuseColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    std::shared_ptr<Light> pointLight = std::make_shared<Light>(std::move(lightProperties));
    pointLight->SetPosition(glm::vec3(10.f, 10.f, 10.f));
    scene->AddLight(pointLight);

    std::vector<std::shared_ptr<aiMaterial>> loadedMaterials;
    std::vector<std::shared_ptr<RenderingObject>> sphereTemplate = MeshLoader::LoadMesh(nullptr, "sphere.obj", &loadedMaterials);
    for (size_t i = 0; i < sphereTemplate.size(); ++i) {
        std::shared_ptr<BlinnPhongShader> shader = std::make_shared<BlinnPhongShader>(shaderSpec, GL_FRAGMENT_SHADER);
        shader->LoadMaterialFromAssimp(loadedMaterials[i]);
        sphereTemplate[i]->SetShader(std::move(shader));

        sphereTemplate[i]->ComputeTangentSpace();
    }

    std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(sphereTemplate);
    sceneObject->Rotate(glm::vec3(SceneObject::GetWorldRight()), PI / 4.f);
    scene->AddSceneObject(sceneObject);
}


void Assignment5::Tick(double deltaTime)
{
}
