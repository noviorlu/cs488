#pragma once

#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <iostream>

class GBuffer {
public:
    GBuffer();
    ~GBuffer();

    void bind() const;
    void unbind() const;
    bool initialize(float width, float height);
    void draw(ShaderProgram &shader) const;
    void drawDepth() const;

    GLuint getPositionTexture() const { return gPosition; }
    GLuint getNormalTexture() const { return gNormal; }
    GLuint getAlbedoIDTexture() const { return gAlbedoID; }
    GLuint getDepthTexture() const { return rboDepth; }

public:
    GLuint gBuffer;
    GLuint gPosition, gNormal, gAlbedoID, rboDepth;
    int width, height;

    GLuint m_vbo_quad;
	GLuint m_vao_quad;
	void initQuad();

};
