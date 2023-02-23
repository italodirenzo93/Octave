#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 uViewPos;
uniform vec3 uColor = vec3(0.04, 0.28, 0.26);
uniform float uShininess = 32.0;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight uDirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uShininess);

    // combine results
    vec3 ambient = light.ambient * uColor;
    vec3 diffuse = light.diffuse * diff * uColor;
    vec3 specular = light.specular * spec * vec3(1.0);

    return ambient + diffuse + specular;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(uViewPos - FragPos);

    vec3 result = CalcDirLight(uDirLight, norm, viewDir);

    FragColor = vec4(result, 1.0);
}
