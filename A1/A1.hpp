// Termm--Fall 2024

#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "maze.hpp"

class A1 : public CS488Window {
public:
	A1();
	virtual ~A1();

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

private:
	void resetParameters();
	void initGrid();
	void initCube();
	void initSphere();
	void createSphere(
		std::vector<float>& vertices, 
		std::vector<unsigned int>& indices, 
		float radius, 
		unsigned int longitudeSegments, 
		unsigned int latitudeSegments
	);

	void screenToCameraRotation(float deltaX, float deltaY);
	void updateView();

	void breakWall(int x, int y);
	void randomMaze();
	void generateMaze();
	void drawMaze();

	// Fields related to the shader and uniforms.
	ShaderProgram m_shader;
	GLint P_uni; // Uniform location for Projection matrix.
	GLint V_uni; // Uniform location for View matrix.
	GLint M_uni; // Uniform location for Model matrix.
	GLint col_uni;   // Uniform location for cube colour.

	// Fields related to grid geometry.
	GLuint m_grid_vao; // Vertex Array Object
	GLuint m_grid_vbo; // Vertex Buffer Object

	// Floor geometry
	GLuint m_floor_vao;
	GLuint m_floor_ibo;

	// Fields related to Cube geometry.
	GLuint m_cube_vao; // Vertex Array Object
	GLuint m_cube_ibo; // Index Buffer Object

	// Fields related to avatar geometry.
	int m_sphere_idx_size; // sphere triangle index size
	GLuint m_sphere_vao; // Vertex Array Object
	GLuint m_sphere_ibo; // Index Buffer Object
	glm::vec3 avatar_pos;
	glm::vec3 prev_avatar_pos;
	enum AvatarMoveDir { STOP, UP, LEFT, DOWN, RIGHT };
	AvatarMoveDir avatar_moveDir = STOP; 
	bool avatar_shift = false;

	// Fields related to maze geometry.
	Maze m;
	int m_idx_size; // maze triangle index size
	GLuint m_maze_vao = 0; // Vertex Array Object
	GLuint m_maze_vbo; // Vertex Buffer Object
	GLuint m_maze_ibo; // Index Buffer Object
	float scaleFactor = 1.0f; // Maze's scale factor

	// Matrices controlling the camera and projection.
	glm::mat4 proj;
	glm::mat4 view;

	// angle of camera control
	float theta;
	float phi;
	float r;

	// camera control variables
	double lastX, lastY;
	double deltaX, deltaY;
	bool LeftMousePressed;
	float persistence_timer = 0.0f;
	float persistence_time = 0.5f;
	float glfw_time = 0.0f;

	float colour[3];
	int current_col;

	float maze_colour[3];
	float floor_colour[3];
	float avatar_colour[3];
	
	bool runState = false;
};
