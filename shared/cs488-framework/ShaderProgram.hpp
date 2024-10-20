/*
 * ShaderProgram
 */

#pragma once

#include "OpenGLImport.hpp"

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class ShaderProgram {
public:
    ShaderProgram();

    ~ShaderProgram();

    void generateProgramObject();

    void attachVertexShader(const char * filePath);
    
    void attachFragmentShader(const char * filePath);
    
    void attachGeometryShader(const char * filePath);

    void link();

    void enable() const;

    void disable() const;

    void recompileShaders();

    GLuint getProgramObject() const;

    GLint getUniformLocation(const std::string& uniformName) const;
    // Set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1iv(const std::string& name, int count, int* value);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void SetUniform4fv(const std::string& name, const glm::vec4& value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform3fv(const std::string& name, const glm::vec3& value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform1f(const std::string& name, float value);

    GLint getAttribLocation(const char * attributeName) const;


private:
    struct Shader {
        GLuint shaderObject;
        std::string filePath;

        Shader();
    };

    Shader vertexShader;
    Shader fragmentShader;
    Shader geometryShader;

    GLuint programObject;
    GLuint prevProgramObject;
    GLuint activeProgram;

    void extractSourceCode(std::string & shaderSource, const std::string & filePath);
    
    void extractSourceCodeAndCompile(const Shader &shader);

    GLuint createShader(GLenum shaderType);

    void compileShader(GLuint shaderObject, const std::string & shader);

    void checkCompilationStatus(GLuint shaderObject);

    void checkLinkStatus();

    void deleteShaders();

    mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};

