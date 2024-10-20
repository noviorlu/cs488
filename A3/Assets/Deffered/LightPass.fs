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

void main() {
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec4 Normal = normalize(texture(gNormal, TexCoords).rgba);
    if(Normal.a == 1.0) return;
    vec4 Albedo = texture(gAlbedoID, TexCoords).rgba;

    FragColor = vec4(Albedo.rgb, 1.0);
    // FragColor = vec4(Albedo, 1.0);

    vec3 lighting = vec3(0.0);

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(lights[i].Position - FragPos);

        float diff = max(dot(Normal.rgb, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].Color;

        lighting += diffuse * Albedo.rgb;
    }

    // FragColor = vec4(lighting, 1.0);
}
