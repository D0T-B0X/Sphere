#version 430 core
in vec3 vWorldPos;
in vec3 vNormal;
out vec4 FragColor;

uniform bool source;
uniform vec3 inColor;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform float ambientStrength = 0.12;
uniform float diffuseStrength = 1.0;
uniform float specularStrength = 0.6;
uniform float shininess = 32.0;

void main() {

    if (source) {
        FragColor = vec4(lightColor, 1.0);
        return;
    }

    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightPos - vWorldPos);
    vec3 V = normalize(viewPos - vWorldPos);

    float diff = max(dot(N, L), 0.0);

    vec3 R = reflect(-L, N);
    float spec = pow(max(dot(R, V), 0.0), shininess);

    vec3 ambient = ambientStrength * inColor;
    vec3 diffuse = diffuseStrength * diff * inColor * lightColor;
    vec3 specular = specularStrength * spec * lightColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}