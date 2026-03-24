
const int AMBIENT = 0;
const int DIRECTIONAL = 1;
const int POSITIONAL = 2;
const int SPOT = 3;


struct LightAttribute {
    vec3 color;
    float intensity;
};


struct Attenuation {
    float range;
    float constant;
    float linear;
    float quadratic;
};


struct Light {
    int type;
    vec3 direction;
    vec3 position;
    LightAttribute ambient;
    LightAttribute diffuse;
    LightAttribute specular;
    Attenuation attenuation;
    float innerCutoff;
    float outerCutoff;
};


struct Lighting {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


float GetAttenuationFactor(Light light, vec3 fragmentPosition) {
    float distance = length(light.position - fragmentPosition);
    return 1.0f / (light.attenuation.constant + distance * (light.attenuation.linear + distance * light.attenuation.quadratic));
}


float GetSpecularIntensity(vec3 N, vec3 H, float shininess) {
    float dotNH = max(dot(N, H), 0.0);
    if (shininess <= 0.0) return 0.0;
    return pow(dotNH, shininess);
}


Lighting NoLighting(void) {
    Lighting lighting;
    lighting.ambient = vec3(0.0f);
    lighting.diffuse = vec3(0.0f);
    lighting.specular = vec3(0.0f);
    return lighting;
}


Lighting Add(Lighting a, Lighting b) {
    Lighting result;
    result.ambient = a.ambient + b.ambient;
    result.diffuse = a.diffuse + b.diffuse;
    result.specular = a.specular + b.specular;
    return result;
}


Lighting EvaluateAmbientLight(Light light) {
    Lighting lighting;
    lighting.ambient = light.ambient.color * light.ambient.intensity;
    lighting.diffuse = vec3(0.0f);
    lighting.specular = vec3(0.0f);
    return lighting;
}


Lighting EvaluateDirectionalLight(Light light, vec3 fragmentNormal, vec3 viewDirection, float shininess) {
    Lighting lighting;
    vec3 N = normalize(fragmentNormal);
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(L + viewDirection);
    float diffuseIntensity = max(0.0f, dot(N, L));
    float specularIntensity = GetSpecularIntensity(N, H, shininess);
    lighting.ambient = light.ambient.color * light.ambient.intensity;
    lighting.diffuse = light.diffuse.color * light.diffuse.intensity * diffuseIntensity;
    lighting.specular = light.specular.color * light.specular.intensity * specularIntensity;
    return lighting;
}


Lighting EvaluatePositionalLight(Light light, vec3 fragmentPosition, vec3 fragmentNormal, vec3 viewDirection, float shininess) {
    Lighting lighting;
    vec3 N = normalize(fragmentNormal);
    vec3 L = normalize(light.position - fragmentPosition);
    vec3 H = normalize(L + viewDirection);
    float attenuation = GetAttenuationFactor(light, fragmentPosition);
    float diffuseIntensity = max(0.0f, dot(N, L));
    float specularIntensity = GetSpecularIntensity(N, H, shininess);
    lighting.ambient = light.ambient.color * light.ambient.intensity * attenuation;
    lighting.diffuse = light.diffuse.color * light.diffuse.intensity * diffuseIntensity * attenuation;
    lighting.specular = light.specular.color * light.specular.intensity * specularIntensity * attenuation;
    return lighting;
}


Lighting EvaluateSpotLight(Light light, vec3 fragmentPosition, vec3 fragmentNormal, vec3 viewDirection, float shininess) {
    Lighting lighting;
    vec3 N = normalize(fragmentNormal);
    vec3 L = normalize(light.position - fragmentPosition);
    vec3 H = normalize(L + viewDirection);
    float attenuation = GetAttenuationFactor(light, fragmentPosition);
    float diffuseIntensity = max(0.0f, dot(N, L));
    float specularIntensity = GetSpecularIntensity(N, H, shininess);
    float theta = dot(normalize(-light.direction), L);
    float epsilon = light.innerCutoff - light.outerCutoff;
    float spotIntensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);
    lighting.ambient = light.ambient.color * light.ambient.intensity * attenuation;
    lighting.diffuse = light.diffuse.color * light.diffuse.intensity * diffuseIntensity * spotIntensity * attenuation;
    lighting.specular = light.specular.color * light.specular.intensity * specularIntensity * spotIntensity * attenuation;
    return lighting;
}


Lighting EvaluateLight(Light light, vec3 fragmentPosition, vec3 fragmentNormal, vec3 viewDirection, float shininess) {
    switch (light.type) {
        case AMBIENT: return EvaluateAmbientLight(light);
        case DIRECTIONAL: return EvaluateDirectionalLight(light, fragmentNormal, viewDirection, shininess);
        case POSITIONAL: return EvaluatePositionalLight(light, fragmentPosition, fragmentNormal, viewDirection, shininess);
        case SPOT: return EvaluateSpotLight(light, fragmentPosition, fragmentNormal, viewDirection, shininess);
        default: return NoLighting();
    }
}
