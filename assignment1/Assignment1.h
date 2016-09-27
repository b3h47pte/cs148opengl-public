#pragma once

#ifndef __ASSIGNMENT_1__
#define __ASSIGNMENT_1__

#include "common/Application.h"

class Assignment1: public Application
{
public:
    Assignment1(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera);

    static std::unique_ptr<Application> CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera);

    virtual glm::vec2 GetWindowSize() const;

    virtual void HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime);
private:
    virtual void SetupScene();

    virtual void SetupExample1();
    virtual void SetupExample2();
    virtual void SetupExample3();
};

#endif
