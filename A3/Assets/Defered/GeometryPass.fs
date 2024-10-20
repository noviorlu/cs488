#version 330 core

layout (location = 0) out vec4 gPosition;  
layout (location = 1) out vec4 geoNormal;
layout (location = 2) out vec4 gAlbedoID;
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
} fs_in;

struct Material {
    vec3 kd;
};
uniform Material material;
uniform int nodeId;
uniform int pickMode;

vec3 intToColor(int picker) {
    float r = float((picker >> 16) & 0xFF) / 255.0;
    float g = float((picker >> 8) & 0xFF) / 255.0;
    float b = float(picker & 0xFF) / 255.0;
    return vec3(r, g, b);
}

void main() {
    gPosition = vec4(fs_in.FragPos, gl_FragCoord.z);
    geoNormal = vec4(normalize(fs_in.Normal), 1.0);
    if(pickMode == 1){
        gAlbedoID = vec4(intToColor(nodeId), 1.0);
    }
    else{
        gAlbedoID = vec4(material.kd, 1.0);
    }
}
