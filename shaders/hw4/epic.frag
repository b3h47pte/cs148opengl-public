#version 330

in vec4 fragmentColor;
in vec4 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 finalColor;

uniform InputMaterial {
    float matRoughness;
    vec4 matSpecular;
    float matMetallic;
} material;

struct LightProperties {
    vec4 diffuseColor;
    vec4 specularColor;
    vec4 directionalLightDir;
    float spotInnerConeAngleDegrees;
    float spotOuterConeAngleDegrees;
};
uniform LightProperties genericLight;

struct PointLight {
    vec4 pointPosition;
};
uniform PointLight pointLight;

uniform vec4 cameraPosition;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform int lightingType;

const float PI = 3.1415926535;

vec4 pointLightSubroutine(vec4 N, vec4 worldPosition, vec3 worldNormal)
{
    // Direction from the surface to the point light
    vec4 L = normalize(pointLight.pointPosition - vertexWorldPosition);
    float NdL = max(0, dot(N,L));

    // Insert code for Section 3.2 here.
    return vec4(NdL);
}

vec4 directionalLightSubroutine(vec4 N, vec4 worldPosition, vec3 worldNormal)
{
    // Insert code for Section 3.3 here.
    return vec4(0.0);
}

vec4 hemisphereLightSubroutine(vec4 N, vec4 worldPosition, vec3 worldNormal)
{
    // Insert code for Section 3.4 here.
    return vec4(0.0);
}

vec4 spotLightSubroutine(vec4 N, vec4 worldPosition, vec3 worldNormal)
{
    // Insert code for Section 3.5 here.
    return vec4(0.0);
}

vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0.0);
}

vec4 attenuateLight(vec4 originalColor)
{
    float lightDistance = length(pointLight.pointPosition - vertexWorldPosition);
    float attenuation = 1.0 / (constantAttenuation + lightDistance * linearAttenuation + lightDistance * lightDistance * quadraticAttenuation);
    return originalColor * attenuation;
}

void main()
{
    // Normal to the surface
    vec4 N = vec4(normalize(vertexWorldNormal), 0.f);

    vec4 lightingColor = vec4(0);
    if (lightingType == 0) {
        lightingColor = globalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 1) {
        lightingColor = attenuateLight(pointLightSubroutine(N, vertexWorldPosition, vertexWorldNormal));
    } else if (lightingType == 2) {
        lightingColor = directionalLightSubroutine(N, vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 3) {
        lightingColor = hemisphereLightSubroutine(N, vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 5) {
        lightingColor = attenuateLight(spotLightSubroutine(N, vertexWorldPosition, vertexWorldNormal));
    }
    finalColor = lightingColor * fragmentColor;
    finalColor.a = 1.0;
}

