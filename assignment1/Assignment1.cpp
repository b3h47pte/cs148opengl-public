#include "assignment1/Assignment1.h"
#include "common/core.h" // <-- haha.

Assignment1::Assignment1(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
}

std::unique_ptr<Application> Assignment1::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment1>(std::move(scene), std::move(camera));
}

glm::vec2 Assignment1::GetWindowSize() const
{
    return glm::vec2(800.f, 800.f);
}

void Assignment1::SetupScene()
{
    SetupExample1();
}

void Assignment1::SetupExample1()
{
    OGL_CALL(glDisable(GL_PROGRAM_POINT_SIZE));
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "basicColor/basicColor.vert" },
        { GL_FRAGMENT_SHADER, "basicColor/basicColor.frag"}
    };
    std::shared_ptr<ShaderProgram> shader = std::make_shared<ShaderProgram>(shaderSpec);

    std::unique_ptr<RenderingObject::PositionArray> vertexPositions = make_unique<RenderingObject::PositionArray>(
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 0.f, 0.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {1.f, 1.f, 0.f, 1.f},
            // Triangle 2
            {0.f, 0.f, 0.f, 1.f},
            {-1.f, 1.f, 0.f, 1.f},
            {-1.f, 0.f, 0.f, 1.f}
        })
    );
    
    std::unique_ptr<RenderingObject::ColorArray> vertexColors = make_unique<RenderingObject::ColorArray>(
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {1.f, 0.f, 0.f, 1.f},
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 0.f, 1.f, 1.f},
            // Triangle 2
            {1.f, 0.f, 0.f, 1.f},
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 0.f, 1.f, 1.f}
        })
    );

    std::shared_ptr<RenderingObject> triangleTemplate = std::make_shared<RenderingObject>(shader, std::move(vertexPositions), nullptr, nullptr, nullptr, std::move(vertexColors));
    std::shared_ptr<SceneObject> sceneTriangle = std::make_shared<SceneObject>(triangleTemplate);
    scene->AddSceneObject(sceneTriangle);
}

void Assignment1::SetupExample2()
{
    OGL_CALL(glEnable(GL_PROGRAM_POINT_SIZE));
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "basicColor/basicColor.vert" },
        { GL_FRAGMENT_SHADER, "basicColor/basicColor.frag"}
    };
    std::shared_ptr<ShaderProgram> shader = std::make_shared<ShaderProgram>(shaderSpec);

    std::unique_ptr<RenderingObject::PositionArray> vertexPositions = make_unique<RenderingObject::PositionArray>(
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 0.f, 0.f, 1.0f},
            {0.9f, 0.f, 0.f, 1.0f},
            {0.9f, 0.9f, 0.f, 1.0f},
            // Triangle 2
            {0.f, 0.f, 0.f, 1.0f},
            {-0.9f, 0.9f, 0.f, 1.0f},
            {-0.9f, 0.f, 0.f, 1.0f}
        })
    );
    
    std::unique_ptr<RenderingObject::ColorArray> vertexColors = make_unique<RenderingObject::ColorArray>(
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 0.f, 1.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            // Triangle 2
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 0.f, 1.f, 1.f},
            {1.f, 0.f, 0.f, 1.f}
        })
    );

    std::shared_ptr<RenderingObject> pointTemplate = std::make_shared<RenderingObject>(shader, std::move(vertexPositions), nullptr, nullptr, nullptr, std::move(vertexColors));
    pointTemplate->SetDrawMode(GL_POINTS);
        
    std::shared_ptr<SceneObject> pointObject = std::make_shared<SceneObject>(pointTemplate);
    scene->AddSceneObject(pointObject);
}

void Assignment1::SetupExample3()
{
    OGL_CALL(glDisable(GL_PROGRAM_POINT_SIZE));

    // Note that this call may or may not do anything depending on the implementation (driver).
    // You can query GL_ALIASED_LINE_WIDTH_RANGE or GL_SMOOTH_LINE_WIDTH_RANGE (if you enable smoothing) 
    // to check the maximum range. On my Mac (~2012, Nvidia GT 650M), the maximum line width is 1!
    OGL_CALL(glLineWidth(4.f));
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "basicColor/basicColor.vert" },
        { GL_FRAGMENT_SHADER, "basicColor/basicColor.frag"}
    };
    std::shared_ptr<ShaderProgram> shader = std::make_shared<ShaderProgram>(shaderSpec);

    std::unique_ptr<RenderingObject::PositionArray> vertexPositions = make_unique<RenderingObject::PositionArray>(
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 0.f, 0.f, 1.0f},
            {0.7f, 0.f, 0.f, 1.0f},

            {0.7f, 0.f, 0.f, 1.0f},
            {0.7f, 0.7f, 0.f, 1.0f},

            {0.7f, 0.7f, 0.f, 1.0f},
            {0.f, 0.f, 0.f, 1.0f},

            // Triangle 2
            {0.f, 0.f, 0.f, 1.0f},
            {-0.7f, 0.7f, 0.f, 1.0f},

            {-0.7f, 0.7f, 0.f, 1.0f},
            {-0.7f, 0.f, 0.f, 1.0f},

            {-0.7f, 0.f, 0.f, 1.0f},
            {0.f, 0.f, 0.f, 1.0f}
        })
    );

   
    std::unique_ptr<RenderingObject::ColorArray> vertexColors = make_unique<RenderingObject::ColorArray>(
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 0.f, 1.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 0.f, 1.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            // Triangle 2
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 0.f, 1.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {0.f, 1.f, 0.f, 1.f},
            {0.f, 0.f, 1.f, 1.f},
            {1.f, 0.f, 0.f, 1.f}
        })
    );

    std::shared_ptr<RenderingObject> pointTemplate = std::make_shared<RenderingObject>(shader, std::move(vertexPositions), nullptr, nullptr, nullptr, std::move(vertexColors));
    pointTemplate->SetDrawMode(GL_LINES);
        
    std::shared_ptr<SceneObject> pointObject = std::make_shared<SceneObject>(pointTemplate);
    scene->AddSceneObject(pointObject);
}

void Assignment1::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{
    if (state != SDL_KEYDOWN || repeat) {
        return;
    }

    switch (key.sym) {
    case SDLK_0:
        scene->ClearScene();
        break;
    case SDLK_1:
        scene->ClearScene();
        SetupExample1();
        break;
    case SDLK_2:
        scene->ClearScene();
        SetupExample2();
        break;
    case SDLK_3:
        scene->ClearScene();
        SetupExample3();
        break;
    default:
        break;
    }
}
