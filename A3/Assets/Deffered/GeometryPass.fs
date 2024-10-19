#version 330 core

layout (location = 0) out vec3 gPosition;  
layout (location = 1) out vec3 gNormal;    
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
    gPosition = fs_in.FragPos;
    gNormal = normalize(fs_in.Normal);
    gAlbedoID = vec4(material.kd, nodeId);
}
