// Termm--Fall 2024

#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
	VertexData();

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};


class A2 : public CS488Window {
public:
	A2();
	virtual ~A2();

protected:
	virtual void init() override;
    virtual void appLogic() override;
    virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();

	void initLineData();

	void setLineColour(const glm::vec3 & colour);

	void drawLine (
			const glm::vec2 & v0,
			const glm::vec2 & v1
	);

	ShaderProgram m_shader;

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

protected:
	void resetParameters();
	
	void initCube();
	void initCoordinate();

    void drawCube(glm::mat4 tf);
	void drawCoordinate(glm::mat4 tf);
	void drawViewport();

	bool nearFarClipping(glm::vec4 &v0, glm::vec4 &v1);
	bool boundaryClipping(glm::vec4 &v0, glm::vec4 &v1);
	static glm::mat4 modelMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);
	static glm::mat4 viewMatrix(const glm::vec3& eye, const glm::vec3& rotation);
	static glm::mat4 orthographicMatrix(float left, float right, float bottom, float top, float near, float far);
	static glm::mat4 projectionMatrix(float fov_y, float aspect, float near, float far);
	void matrixConstruction();
	void calculateViewport();
	void ndcNormalize(glm::vec4 &v){ v /= abs(v.w); }
	
	void rotateViewHandler(double deltaX, double deltaY);
	void translateViewHandler(double deltaX, double deltaY);
	void perspectiveHandler(double deltaX, double deltaY);
	void rotateModelHandler(double deltaX, double deltaY);
	void translateModelHandler(double deltaX, double deltaY);
	void scaleModelHandler(double deltaX, double deltaY);

protected:
	// coordinate
	std::vector<glm::vec4> m_coordinateVertices;
	std::vector<glm::vec2> m_coordinateLines;

	// Cube
	std::vector<glm::vec4> m_cubeVertices;
	std::vector<glm::vec2> m_cubeLines;

	glm::vec3 m_cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_cubeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 m_cubeRotate = glm::vec3(0.0f, 0.0f, 0.0f); // range [-180, 180]

	// Freelook camera
	glm::vec3 m_camPos = glm::vec3(5.0f, 0.0f, 0.0f);
	glm::vec3 m_camRotate = glm::vec3(0.0f, 0.0f, 0.0f);
	float m_near;
	float m_far;

	bool isOrtho;
	float m_fov;
	float m_aspect;

	float m_left;
	float m_right;
	float m_bottom;
	float m_top;

	// Matrix construction
	bool m_modelFlag = false;
	bool m_viewFlag = false;
	bool m_orthoFlag = false;
	bool m_projFlag = false;

	glm::mat4 m_modelMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_orthoMatrix;
	glm::mat4 m_projMatrix;

	// Mouse Control
	const char* controlModeNames[8] = {
		"NA", "ROTATEVIEW", "TRANSLATEVIEW", "PERSPECTIVE", "ROTATEMODEL", "TRANSLATEMODEL", "SCALEMODEL", "VIEWPORT"
	};
	const char* mouseButtonNames[4] = {
	"NONE", "LEFT", "MIDDLE", "RIGHT"
	};
	enum ControlMode{
		NA = 0,
		ROTATEVIEW,
		TRANSLATEVIEW,
		PERSPECTIVE,
		ROTATEMODEL,
		TRANSLATEMODEL,
		SCALEMODEL,
		VIEWPORT
	};
	ControlMode m_controlMode = NA;
	enum MouseButton{
		NONE = 0,
		LEFT,
		RIGHT,
		MIDDLE
	};

	MouseButton m_mouseButton = NONE;
	glm::dvec2 m_prevMousePos;
	glm::dvec2 m_pressedMousePos;

	int m_windowWidth;
	int m_windowHeight;

	// view port
	glm::dvec2 m_viewportStart;
	glm::dvec2 m_viewportEnd;
	float m_viewportLeft, m_viewportRight, m_viewportBottom, m_viewportTop;

	bool isDebug = false;
};
