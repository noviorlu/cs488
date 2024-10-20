#version 330 core

layout (location = 0) out vec4 gPosition;  
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoID;
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

struct Material {
    vec3 kd;
};
uniform Material material;
uniform float nodeId;

void main() {
    gPosition = vec4(fs_in.FragPos, nodeId);
    gNormal = vec4(normalize(fs_in.Normal), 0.0);
    gAlbedoID = vec4(material.kd, 1.0);
}
