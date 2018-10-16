#pragma once

#ifndef __ASSIGNMENT_4__
#define __ASSIGNMENT_4__

#include "common/Application.h"

class Assignment4: public Application
{
public:
    Assignment4(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);

    static std::unique_ptr<Application> CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera);
    static std::shared_ptr<class Camera> CreateCamera();

    virtual glm::vec2 GetWindowSize() const;

    virtual void HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime);
    virtual void Tick(double deltaTime);
protected:
    virtual void HandleWindowResize(float x, float y);

private:
    virtual void SetupScene();

    virtual void SetupExample1();
    virtual void GenericSetupExample(std::shared_ptr<class ShaderProgram> shader, std::shared_ptr<ShaderProgram> groundShader);

    virtual void SetupCamera();

    std::shared_ptr<class Light> sunLight;
    std::shared_ptr<class Light> hemisphereLight;
    std::shared_ptr<class Light> pointLight;
    std::shared_ptr<class Light> spotLight;
    std::vector<std::shared_ptr<class SceneObject>> sphereDance;
};

#endif
