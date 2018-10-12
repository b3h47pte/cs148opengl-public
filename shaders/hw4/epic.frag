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


vec4 pointLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    // Normal to the surface
    vec4 N = vec4(normalize(worldNormal), 0.f);
    
    // Direction from the surface to the point light
    vec4 L = normalize(pointLight.pointPosition - worldPosition);

    return vec4(max(0, dot(N,L)));
}

vec4 directionalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0.0);
}

vec4 hemisphereLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0.0);
}

vec4 globalLightSubroutine(vec4 worldPosition, vec3 worldNormal)
{
    return vec4(0.0);
}

vec4 attenuateLight(vec4 originalColor, vec4 worldPosition)
{
    float lightDistance = length(pointLight.pointPosition - worldPosition);
    float attenuation = 1.0 / (constantAttenuation + lightDistance * linearAttenuation + lightDistance * lightDistance * quadraticAttenuation);
    return originalColor * attenuation;
}

void main()
{
    vec4 lightingColor = vec4(0);
    if (lightingType == 0) {
        lightingColor = globalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 1) {
        lightingColor = pointLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 2) {
        lightingColor = directionalLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    } else if (lightingType == 3) {
        lightingColor = hemisphereLightSubroutine(vertexWorldPosition, vertexWorldNormal);
    }
    finalColor = attenuateLight(lightingColor, vertexWorldPosition) * fragmentColor;
}

