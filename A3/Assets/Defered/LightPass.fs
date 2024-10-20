#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 TransformedLightPositions[10];

uniform sampler2D gPosition;
uniform sampler2D geoNormal;
uniform sampler2D gAlbedoID;

struct Light {
    vec3 Color;
};

uniform int numLights;
uniform int pickMode;
uniform Light lights[10];

void main() {
    vec4 FragPos = texture(gPosition, TexCoords).rgba;
    vec4 Normal = texture(geoNormal, TexCoords).rgba;
    vec4 Albedo = texture(gAlbedoID, TexCoords).rgba;

    // depth testing
    if(FragPos.a == 1.0){
        FragColor = vec4(0.0);
        return;
    }

    // nodeID when picking mode
    if(pickMode != 0){
        FragColor = Albedo;
        return;
    }

    vec3 lighting = vec3(0.0);

    for (int i = 0; i < numLights; ++i) {
        vec3 lightDir = normalize(TransformedLightPositions[i] - FragPos.rgb);
        float diff = max(dot(Normal.rgb, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].Color;

        lighting += diffuse * Albedo.rgb;
    }

    FragColor = vec4(lighting, 1.0);
}
