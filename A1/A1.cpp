// Termm--Fall 2024

#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>

#include <sys/types.h>
#include <unistd.h>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

static const size_t DIM = 16;

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col( 0 ), m(DIM)
{
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{
	// if(m_maze) delete m_maze;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Initialize random number generator
	int rseed=getpid();
	srandom(rseed);
	// Print random number seed in case we want to rerun with
	// same random numbers
	cout << "Random number seed = " << rseed << endl;
	
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
		getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

	initGrid();
	initCube();
	initSphere();
	
	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	resetParameters();
	proj = glm::perspective( 
		glm::radians( 30.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );
}

void A1::resetParameters(){
	theta = 90.0;
	phi = 26.565;
	r = sqrt(10) * DIM;
	updateView();

	maze_colour[0] = 0.4f;
	maze_colour[1] = 0.4f;
	maze_colour[2] = 0.4f;

	floor_colour[0] = 0.2f;
	floor_colour[1] = 0.2f;
	floor_colour[2] = 0.2f;

	avatar_colour[0] = 0.9f;
	avatar_colour[1] = 0.9f;
	avatar_colour[2] = 0.9f;

	avatar_pos = glm::vec3(-1, 0, -1);
	runState = false;
}

void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	// generate floor
	float floorVertices[] = {
		0.0f, 0.0f, 0.0f, // 0
		DIM, 0.0f, 0.0f, // 1
		DIM, 0.0f, DIM, // 2
		0.0f, 0.0f, DIM  // 3
	};
	
	unsigned int floorIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	glGenVertexArrays(1, &m_floor_vao);
	glBindVertexArray(m_floor_vao);

	GLuint m_floor_vbo;
	glGenBuffers(1, &m_floor_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_floor_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_floor_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floor_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

void A1::initCube(){
	float cubeVertices[] = {
		0.0f, 0.0f, 0.0f, // 0
		1.0f, 0.0f, 0.0f, // 1
		1.0f, 1.0f, 0.0f, // 2
		0.0f, 1.0f, 0.0f, // 3
		0.0f, 0.0f, 1.0f, // 4
		1.0f, 0.0f, 1.0f, // 5
		1.0f, 1.0f, 1.0f, // 6
		0.0f, 1.0f, 1.0f  // 7
	};

	unsigned int cubeIndices[] = {
		// Front face
		0, 1, 2,
		2, 3, 0,
		// Back face
		4, 5, 6,
		6, 7, 4,
		// Left face
		0, 3, 7,
		7, 4, 0,
		// Right face
		1, 5, 6,
		6, 2, 1,
		// Top face
		3, 2, 6,
		6, 7, 3,
		// Bottom face
		0, 4, 5,
		5, 1, 0
	};

    glGenVertexArrays(1, &m_cube_vao);
    glBindVertexArray(m_cube_vao);

	GLuint m_cube_vbo;
    glGenBuffers(1, &m_cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_cube_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cube_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
	glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void A1::initSphere(){
	// Define sphere parameters
    float radius = 1.0f;
    unsigned int longitudeSegments = 50; // Controls the horizontal segments
    unsigned int latitudeSegments = 50;  // Controls the vertical segments

    // Create vertex and index buffers
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    createSphere(vertices, indices, radius, longitudeSegments, latitudeSegments);
	m_sphere_idx_size = indices.size();

    // Create and bind a m_sphere_vao, VBO, and m_sphere_ibo
    unsigned int VBO;
    glGenVertexArrays(1, &m_sphere_vao);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &m_sphere_ibo);

    glBindVertexArray(m_sphere_vao);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphere_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Specify the layout of the vertex data (Position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void A1::createSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, unsigned int longitudeSegments, unsigned int latitudeSegments) {
    for (unsigned int lat = 0; lat <= latitudeSegments; ++lat) {
        float theta = lat * M_PI / latitudeSegments;
        float sinTheta = std::sin(theta);
        float cosTheta = std::cos(theta);

        for (unsigned int lon = 0; lon <= longitudeSegments; ++lon) {
            float phi = lon * 2.0f * M_PI / longitudeSegments;
            float sinPhi = std::sin(phi);
            float cosPhi = std::cos(phi);

            // Vertex position (x, y, z)
            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            // Push vertex coordinates
            vertices.push_back(radius * x); // X
            vertices.push_back(radius * y); // Y
            vertices.push_back(radius * z); // Z
        }
    }

    for (unsigned int lat = 0; lat < latitudeSegments; ++lat) {
        for (unsigned int lon = 0; lon < longitudeSegments; ++lon) {
            unsigned int first = (lat * (longitudeSegments + 1)) + lon;
            unsigned int second = first + longitudeSegments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void A1::breakWall(int x, int y){
	m.setValue(x, y, 0);
	generateMaze();
}

void A1::randomMaze(){
	m.digMaze();
	m.printMaze();
	A1::generateMaze();
	avatar_pos = glm::vec3(m.getStart().y, 0, m.getStart().x);
	runState = true;
}

void A1::generateMaze(){
	m.generateGeometry();
	auto vertices = m.getVertices();
	auto triangles = m.getTriangles();

	m_idx_size = triangles.size() * 3;

	// Cleanup previous buffers
	// do safety check if buffers are already created
	if (m_maze_vao != 0) {
		glDeleteVertexArrays(1, &m_maze_vao);
		glDeleteBuffers(1, &m_maze_vbo);
		glDeleteBuffers(1, &m_maze_ibo);
	}

	glGenVertexArrays(1, &m_maze_vao);
	glBindVertexArray(m_maze_vao);

	glGenBuffers(1, &m_maze_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_maze_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_maze_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_maze_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(glm::uvec3), triangles.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
}

void A1::drawMaze(){
	// Draw the maze
	glBindVertexArray(m_maze_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_maze_ibo);
	glDrawElements(GL_TRIANGLES, m_idx_size, GL_UNSIGNED_INT, 0);
}

void A1::updateView(){
	float radianTheta = glm::radians(theta);
	float radianPhi = glm::radians(phi);

	float x = r * sin(radianPhi) * cos(radianTheta);
	float y = r * cos(radianPhi);
	float z = r * sin(radianPhi) * sin(radianTheta);

	view = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
	// avatar movement
	if(runState && avatar_moveDir != STOP){
		// discrete movement, move 1 unit at a time
		float move = 1.0f;
		prev_avatar_pos = avatar_pos;
		switch(avatar_moveDir){
			case UP:
				avatar_pos.z -= move;
				break;
			case DOWN:
				avatar_pos.z += move;
				break;
			case LEFT:
				avatar_pos.x -= move;
				break;
			case RIGHT:
				avatar_pos.x += move;
				break;
			default:
				break;
		}
		avatar_moveDir = STOP;
		if(
			avatar_pos.x < 0 || avatar_pos.x >= DIM || 
			avatar_pos.z < 0 || avatar_pos.z >= DIM
		){
			avatar_pos = prev_avatar_pos;
		}
		else if(m.getValue(avatar_pos.z, avatar_pos.x) == 1){
			if(avatar_shift){
				// break the wall
				breakWall(avatar_pos.z, avatar_pos.x);
			}
			else{
				avatar_pos = prev_avatar_pos;
			}
		}
	}

	// camera Persistence rotation
	double cur_time = glfwGetTime();

	if(isOrbitView){
		if(persistence_timer > 0){
			
			persistence_timer -= cur_time - glfw_time;

			if(persistence_timer < 0){
				persistence_timer = 0;
				deltaX = 0;
				deltaY = 0;
			}

			// lerp deltaX and deltaY to 0 a+t*(b-a)
			float dx = deltaX * (persistence_timer / persistence_time);
			float dy = deltaY * (persistence_timer / persistence_time);
			screenToCameraRotation(dx, dy);
		}
	}
	else{
		if(abs(presistentX) > 0.01){
			// camera rotation
			screenToCameraRotation(presistentX * 0.01, 0);
		}
	}

	glfw_time = cur_time;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for 
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		// reset button
		if(ImGui::Button("Reset")){
			resetParameters();
		}
		// dig button
		if(ImGui::Button("Random Maze")){
			randomMaze();
		}
		// persistant view switch
		if(ImGui::Checkbox("Orbit View", &isOrbitView)){
			presistentX = 0;
		}

		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.
		ImGui::ColorEdit3("Maze Colour", maze_colour);
		ImGui::SameLine();
		if( ImGui::RadioButton( "##Maze Col", &current_col, 1 ) ) {
			// Select this colour.
		}

		ImGui::ColorEdit3("Floor Colour", floor_colour);
		ImGui::SameLine();
		if( ImGui::RadioButton( "##Floor Col", &current_col, 2 ) ) {
			// Select this colour.
		}

		ImGui::ColorEdit3("Avatar Colour", avatar_colour);
		ImGui::SameLine();
		if( ImGui::RadioButton( "##Avatar Col", &current_col, 3 ) ) {
			// Select this colour.
		}

		// camera angle control window
		ImGui::Text("Camera Control");
		if(ImGui::SliderFloat("Theta", &theta, -180.0f, 180.0f)){
			updateView();
		}
		if(ImGui::SliderFloat("Phi", &phi, 1.0f, 90.0f)){
			updateView();
		}
		if(ImGui::SliderFloat("Radius", &r, 1.0f, 100.0f)){
			updateView();
		}
		ImGui::SliderFloat("Scale Factor", &scaleFactor, 0.5f, 5.0f);


/*
		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in 
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}
*/
		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
	W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );

	m_shader.enable();
		glEnable( GL_DEPTH_TEST );

		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );

		// Just draw the grid for now.
		glBindVertexArray( m_grid_vao );
		glUniform3f( col_uni, 1, 1, 1 );
		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );

		if(runState){
			// Draw the floor
			glBindVertexArray(m_floor_vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_floor_ibo);
			glUniform3f( col_uni, floor_colour[0], floor_colour[1], floor_colour[2]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// Draw the maze
			glUniform3f( col_uni, maze_colour[0], maze_colour[1], maze_colour[2]);
			glUniformMatrix4fv(
				M_uni, 1, GL_FALSE, 
				value_ptr(
					W * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, scaleFactor, 1.0f))
				)
			);
			drawMaze();
		}

		// Draw the avatar
		glUniform3f( col_uni, avatar_colour[0], avatar_colour[1], avatar_colour[2]);
		glUniformMatrix4fv(
			M_uni, 1, GL_FALSE, 
			value_ptr(
				W * glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f)) 
				* glm::translate(glm::mat4(1.0f), avatar_pos) 
				* glm::scale(glm::mat4(1.0f), glm::vec3(0.5f))
			)
		);
		glBindVertexArray(m_sphere_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_sphere_ibo);
		glDrawElements(GL_TRIANGLES, m_sphere_idx_size, GL_UNSIGNED_INT, 0);

		// Highlight the active square.
	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

void A1::screenToCameraRotation(float deltaX, float deltaY){
	float sensitivity = 0.004 * r;
	deltaX *= sensitivity;
	deltaY *= sensitivity;

	theta += deltaX;
	phi -= deltaY;

	// Clamp phi between 0 and 90
	if (phi > 90.0f) phi = 90.0f;
	else if (phi < 1.0f) phi = 1.0f;

	// Clamp theta between -180 and 180
	if (theta < -180.0f) theta = 180.0f;
	else if (theta > 180.0f) theta = -180.0f;

	updateView();
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A1::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.
		if(LeftMousePressed){
			if(isOrbitView){
				deltaX = xPos - lastX;
				deltaY = yPos - lastY;
				screenToCameraRotation(deltaX, deltaY);
				lastX = xPos;
				lastY = yPos;
			}
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
		if(button == GLFW_MOUSE_BUTTON_LEFT) {
			if(actions == GLFW_PRESS) {
				LeftMousePressed = true;
				
				// orbit rotation
				glfwGetCursorPos(m_window, &lastX, &lastY);
				
				// persistant rotation
				lastCursorPos = glm::dvec2(lastX, lastY);
			} 
			else if(actions == GLFW_RELEASE) {
				LeftMousePressed = false;

				// orbit rotation
				persistence_timer = persistence_time;
				glfw_time = glfwGetTime();

				// persistant rotation
				glm::dvec2 curPos;
				glfwGetCursorPos(m_window, &(curPos.x), &(curPos.y));
				presistentX = curPos.x - lastCursorPos.x;
			}
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Zoom in or out.
	r -= yOffSet;
	if (r < 10.0f) r = 10.0f;
	else if (r > 100.0f) r = 100.0f;
	updateView();

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_PRESS ) {
		switch (key) {
			case GLFW_KEY_SPACE:
				scaleFactor += 1.0f;
				if (scaleFactor > 10.0f) scaleFactor = 10.0f;
				break;
			case GLFW_KEY_BACKSPACE:
				scaleFactor -= 1.0f;
				if (scaleFactor < 0.0f) scaleFactor = 0.01f;
				break;
			case GLFW_KEY_UP:
				avatar_moveDir = UP;
				break;
			case GLFW_KEY_DOWN:
				avatar_moveDir = DOWN;
				break;
			case GLFW_KEY_LEFT:
				avatar_moveDir = LEFT;
				break;
			case GLFW_KEY_RIGHT:
				avatar_moveDir = RIGHT;
				break;
			case GLFW_KEY_D:
				randomMaze();
				break;
			case GLFW_KEY_R:
				resetParameters();
				break;
			case GLFW_KEY_LEFT_SHIFT:
				avatar_shift = true;
				break;
			case GLFW_KEY_Q:
				glfwSetWindowShouldClose(m_window, GL_TRUE);
				break;
		}
	}

	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_LEFT_SHIFT:
				avatar_shift = false;
				break;
		}
	}

	return eventHandled;
}
