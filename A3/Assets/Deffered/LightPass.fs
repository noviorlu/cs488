#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoID;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform int numLights;
uniform Light lights[10];
uniform vec3 viewPos;

void main() {
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 Albedo = texture(gAlbedoID, TexCoords).rgb;

    FragColor = vec4(Normal, 1.0);
    FragColor = vec4(normalize(FragPos), 1.0);
    FragColor = vec4(Albedo, 1.0);
    return;

    // vec3 lighting = vec3(0.0);

    // for (int i = 0; i < numLights; ++i) {
    //     vec3 lightDir = normalize(lights[i].Position - FragPos);

    //     float diff = max(dot(Normal, lightDir), 0.0);
    //     vec3 diffuse = diff * lights[i].Color;

    //     vec3 viewDir = normalize(viewPos - FragPos);
    //     vec3 reflectDir = reflect(-lightDir, Normal);
    //     float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0) * SpecularStrength;
    //     vec3 specular = lights[i].Color * spec;

    //     lighting += (diffuse + specular) * Albedo;
    // }

    // FragColor = vec4(lighting, 1.0);
}
