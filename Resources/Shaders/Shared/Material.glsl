

struct Material {
    bool useDiffuseMap;
    bool useSpecularMap;
    sampler2D diffuseMap;
    sampler2D specularMap;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

