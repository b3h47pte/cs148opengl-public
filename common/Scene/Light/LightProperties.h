#pragma once

#ifndef __LIGHT_PROPERTIES__
#define __LIGHT_PROPERTIES__

#include "common/common.h"

struct LightProperties {
    virtual ~LightProperties() {}

    glm::vec4 diffuseColor;
    glm::vec4 specularColor;
};

struct SpotlightProperties: public LightProperties {
    float innerConeAngleDegrees{0.f};
    float outerConeAngleDegrees{0.f};
};

#endif
