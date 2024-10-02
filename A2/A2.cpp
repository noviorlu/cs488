// Termm--Fall 2024

#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f))
{

}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	// Set the background colour.
	glClearColor(0.1, 0.1, 0.1, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();

	initCube();
	initCoordinate();

	resetParameters();
	matrixConstruction();

	glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
}

void A2::initCube(){
	// init Cube's vertices, lines
	m_cubeVertices.push_back(glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f));
	m_cubeVertices.push_back(glm::vec4(0.5f, -0.5f, -0.5f, 1.0f));
	m_cubeVertices.push_back(glm::vec4(0.5f, 0.5f, -0.5f, 1.0f));
	m_cubeVertices.push_back(glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f));
	m_cubeVertices.push_back(glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f));
	m_cubeVertices.push_back(glm::vec4(0.5f, -0.5f, 0.5f, 1.0f));
	m_cubeVertices.push_back(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	m_cubeVertices.push_back(glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f));

	m_cubeLines.push_back(glm::vec2(0, 1));
	m_cubeLines.push_back(glm::vec2(1, 2));
	m_cubeLines.push_back(glm::vec2(2, 3));
	m_cubeLines.push_back(glm::vec2(3, 0));
	m_cubeLines.push_back(glm::vec2(4, 5));
	m_cubeLines.push_back(glm::vec2(5, 6));
	m_cubeLines.push_back(glm::vec2(6, 7));
	m_cubeLines.push_back(glm::vec2(7, 4));
	m_cubeLines.push_back(glm::vec2(0, 4));
	m_cubeLines.push_back(glm::vec2(1, 5));
	m_cubeLines.push_back(glm::vec2(2, 6));
	m_cubeLines.push_back(glm::vec2(3, 7));
}

void A2::initCoordinate(){
	// init Coordinate's vertices, lines
	m_coordinateVertices.push_back(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	m_coordinateVertices.push_back(glm::vec4(0.2f, 0.0f, 0.0f, 1.0f));
	m_coordinateVertices.push_back(glm::vec4(0.0f, 0.2f, 0.0f, 1.0f));
	m_coordinateVertices.push_back(glm::vec4(0.0f, 0.0f, 0.2f, 1.0f));

	m_coordinateLines.push_back(glm::vec2(0, 1));
	m_coordinateLines.push_back(glm::vec2(0, 2));
	m_coordinateLines.push_back(glm::vec2(0, 3));
}

void A2::resetParameters(){
	m_near = 1.0f;
	m_far = 10.0f;
	m_fov = 30.0f;
	m_aspect = 1.0f;
	m_left = -1.0f;
	m_right = 1.0f;
	m_bottom = -1.0f;
	m_top = 1.0f;
	m_viewportStart = glm::vec2( m_windowWidth * 0.05, m_windowHeight * 0.05 );
	m_viewportEnd = glm::vec2( m_windowWidth * 0.95, m_windowHeight * 0.95 );
	calculateViewport();

	m_modelFlag = true;
	m_viewFlag = true;
	m_projFlag = true;
	m_orthoFlag = true;

	isOrtho = false;
	m_cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_cubeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_cubeRotate = glm::vec3(0.0f, 0.0f, 0.0f);
	m_camPos = glm::vec3(5.0f, 0.0f, 0.0f);
	m_camRotate = glm::vec3(0.0f, 0.0f, 0.0f);

	m_controlMode = NA;
	m_mouseButton = NONE;

	isDebug = false;
}


//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//---------------------------------------------------------------------------------------- Fall 2022
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & V0,   // Line Start (NDC coordinate)
		const glm::vec2 & V1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = V0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = V1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}


glm::mat4 A2::modelMatrix(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glm::mat4 translationMatrix = glm::mat4(1.0f);
    translationMatrix[3][0] = translation.x;
    translationMatrix[3][1] = translation.y;
    translationMatrix[3][2] = translation.z;

    float radX = glm::radians(rotation.x);
    float radY = glm::radians(rotation.y);
    float radZ = glm::radians(rotation.z);

    glm::mat4 rotationMatrixX = glm::mat4(1.0f);
    rotationMatrixX[1][1] = cos(radX);
    rotationMatrixX[1][2] = -sin(radX);
    rotationMatrixX[2][1] = sin(radX);
    rotationMatrixX[2][2] = cos(radX);

    glm::mat4 rotationMatrixY = glm::mat4(1.0f);
    rotationMatrixY[0][0] = cos(radY);
    rotationMatrixY[0][2] = sin(radY);
    rotationMatrixY[2][0] = -sin(radY);
    rotationMatrixY[2][2] = cos(radY);

    glm::mat4 rotationMatrixZ = glm::mat4(1.0f);
    rotationMatrixZ[0][0] = cos(radZ);
    rotationMatrixZ[0][1] = -sin(radZ);
    rotationMatrixZ[1][0] = sin(radZ);
    rotationMatrixZ[1][1] = cos(radZ);

    glm::mat4 rotationMatrix = rotationMatrixZ * rotationMatrixY * rotationMatrixX;

    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix[0][0] = scale.x;
    scaleMatrix[1][1] = scale.y;
    scaleMatrix[2][2] = scale.z;

    modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    return modelMatrix;
}

glm::mat4 A2::viewMatrix(const glm::vec3& eye, const glm::vec3& rotation) {
	float pitch = glm::radians(rotation.x);
	float yaw = glm::radians(rotation.y);
	float roll = glm::radians(rotation.z);
	
	glm::vec3 front;
	front.x = cos(yaw) * cos(pitch);
	front.y = sin(pitch);
	front.z = sin(yaw) * cos(pitch);
	front = glm::normalize(front);

	glm::vec3 up;
	if(rotation.x < -90 || rotation.x > 90){
		up = glm::vec3(0.0f, -1.0f, 0.0f);
	}
	else{
		up = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::vec3 right = glm::normalize(glm::cross(front, up));
	glm::vec3 upDir = glm::cross(right, front);

	// Rodrigues’ rotation formula
	right = right * cos(roll) + glm::cross(front, right) * sin(roll) + front * glm::dot(front, right) * (1 - cos(roll));
	upDir = upDir * cos(roll) + glm::cross(front, upDir) * sin(roll) + front * glm::dot(front, upDir) * (1 - cos(roll));

	glm::mat4 view = glm::mat4(1.0f);
	view[0][0] = right.x;
	view[1][0] = right.y;
	view[2][0] = right.z;
	view[0][1] = upDir.x;
	view[1][1] = upDir.y;
	view[2][1] = upDir.z;
	view[0][2] = -front.x;
	view[1][2] = -front.y;
	view[2][2] = -front.z;
	view[3][0] = -glm::dot(right, eye);
	view[3][1] = -glm::dot(upDir, eye);
	view[3][2] = glm::dot(front, eye);

    return view;
}

glm::mat4 A2::orthographicMatrix(float left, float right, float bottom, float top, float near, float far) {
    glm::mat4 ortho = glm::mat4(1.0f);
    
    ortho[0][0] = 2.0f / (right - left);
    ortho[1][1] = 2.0f / (top - bottom);
    ortho[2][2] = -2.0f / (far - near);
    
    ortho[3][0] = -(right + left) / (right - left);
    ortho[3][1] = -(top + bottom) / (top - bottom);
    ortho[3][2] = -(far + near) / (far - near);
    
    return ortho;
}

glm::mat4 A2::projectionMatrix(float fov_y, float aspect, float near, float far) {
    float f = 1.0f / tan(fov_y / 2.0f);
    float a = aspect;
    float n = near;
    float fr = far;

    glm::mat4 proj = glm::mat4(0.0f);
    proj[0][0] = f / a;
    proj[1][1] = f;
    proj[2][2] = -(fr + n) / (fr - n);
    proj[2][3] = -1.0f;
    proj[3][2] = -2.0f * fr * n / (fr - n);
    
    return proj;
}

void A2::matrixConstruction(){
	if(m_modelFlag){
		m_modelMatrix = modelMatrix(m_cubePos, m_cubeRotate, m_cubeScale);
		m_modelFlag = false;
	}
	if(m_viewFlag){
		m_viewMatrix = viewMatrix(m_camPos, m_camRotate);
		m_viewFlag = false;
	}
	if(m_orthoFlag){
		m_orthoMatrix = orthographicMatrix(m_viewportLeft, m_viewportRight, m_viewportBottom, m_viewportTop, m_near, m_far);
		m_orthoFlag = false;
	}
	if(m_projFlag){
		m_projMatrix = projectionMatrix(glm::radians(m_fov), m_aspect, m_near, m_far);
		m_projFlag = false;
	}
}

void A2::calculateViewport(){
	float x1 = m_viewportStart.x / m_windowWidth * 2 - 1;
	float y1 = 1 - m_viewportStart.y / m_windowHeight * 2;
	float x2 = m_viewportEnd.x / m_windowWidth * 2 - 1;
	float y2 = 1 - m_viewportEnd.y / m_windowHeight * 2;

	m_viewportLeft = std::min(x1, x2);
	m_viewportRight = std::max(x1, x2);
	m_viewportBottom = std::min(y1, y2);
	m_viewportTop = std::max(y1, y2);
}


// return true if the line is clipped
bool A2::nearFarClipping(glm::vec4 &v0, glm::vec4 &v1){
	if((v0.z < m_near && v1.z < m_near) || (v0.z > m_far && v1.z > m_far)){
		return true;
	}

	float factor = abs(v0.z - v1.z);
	if(factor == 0) return false;

	if(v0.z < m_near){
		float t = (m_near - v0.z) / factor;
		v0 = v0 + (v1 - v0) * t;
	}
	else if(v1.z < m_near){
		float t = (m_near - v1.z) / factor;
		v1 = v1 + (v0 - v1) * t;
	}

	if(v0.z > m_far){
		float t = (v0.z - m_far) / factor;
		v0 = v0 + (v1 - v0) * t;
	}
	else if(v1.z > m_far){
		float t = (v1.z - m_far) / factor;
		v1 = v1 + (v0 - v1) * t;
	}
	return false;
}


bool A2::boundaryClipping(glm::vec4 &v0, glm::vec4 &v1) {
	auto clip = [](float p, float q, float &t0, float &t1) -> bool {
		if (p == 0) return q >= 0;
		float r = q / p;
		if (p < 0) {
			if (r > t1) return false;
			if (r > t0) t0 = r;
		} else {
			if (r < t0) return false;
			if (r < t1) t1 = r;
		}
		return true;
	};

	float t0 = 0.0f, t1 = 1.0f;
	glm::vec4 d = v1 - v0;

	if (clip(-d.x, v0.x + 1, t0, t1) && clip(d.x, 1 - v0.x, t0, t1) &&
		clip(-d.y, v0.y + 1, t0, t1) && clip(d.y, 1 - v0.y, t0, t1)) {
		if (t1 < 1) v1 = v0 + t1 * d;
		if (t0 > 0) v0 = v0 + t0 * d;
		
		// project to viewport
		v0.x = m_viewportLeft + (m_viewportRight - m_viewportLeft) * (v0.x + 1) / 2;
		v0.y = m_viewportBottom + (m_viewportTop - m_viewportBottom) * (v0.y + 1) / 2;
		v1.x = m_viewportLeft + (m_viewportRight - m_viewportLeft) * (v1.x + 1) / 2;
		v1.y = m_viewportBottom + (m_viewportTop - m_viewportBottom) * (v1.y + 1) / 2;
		
		return false;
	}
	return true;
}


//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();
	matrixConstruction();
    drawCube(m_modelMatrix);
	drawCoordinate(m_modelMatrix);
	drawCoordinate(glm::mat4(1.0f));
	drawViewport();
}

void A2::drawCube(glm::mat4 tf)
{
    setLineColour(vec3(1.0f, 0.7f, 0.8f));
    for (auto line : m_cubeLines)
    {
        glm::vec4 v0 = m_viewMatrix * tf * m_cubeVertices[line.x];
        glm::vec4 v1 = m_viewMatrix * tf * m_cubeVertices[line.y];

        // nearFar Clipping
        if (nearFarClipping(v0, v1)) continue;

		if(isOrtho){
			v0 = m_orthoMatrix * v0;
			v1 = m_orthoMatrix * v1;
		}
		else{
			v0 = m_projMatrix * v0;
			v1 = m_projMatrix * v1;

			ndcNormalize(v0);
			ndcNormalize(v1);
		}

		if(boundaryClipping(v0, v1)) continue;

		drawLine(glm::vec2(v0), glm::vec2(v1));
    }
}

void A2::drawCoordinate(glm::mat4 tf)
{
	int i = 0;
	for(auto line : m_coordinateLines){
		if(i == 0){
			setLineColour(vec3(1.0f, 0.0f, 0.0f));
		}else if(i == 1){
			setLineColour(vec3(0.0f, 1.0f, 0.0f));
		}else{
			setLineColour(vec3(0.0f, 0.0f, 1.0f));
		}
		i++;
		glm::vec4 v0 = m_viewMatrix * tf * m_coordinateVertices[line.x];
        glm::vec4 v1 = m_viewMatrix * tf * m_coordinateVertices[line.y];

        // nearFar Clipping
        if (nearFarClipping(v0, v1)) continue;

		if(isOrtho){
			v0 = m_orthoMatrix * v0;
			v1 = m_orthoMatrix * v1;
		}
		else{
			v0 = m_projMatrix * v0;
			v1 = m_projMatrix * v1;

			ndcNormalize(v0);
			ndcNormalize(v1);
		}

		if(boundaryClipping(v0, v1)) continue;

		drawLine(glm::vec2(v0), glm::vec2(v1));
	}
}

void A2::drawViewport()
{
	setLineColour(vec3(0.2f, 1.0f, 1.0f));
	// draw viewport base on m_viewportStart and m_viewportEnd
	drawLine(glm::vec2(m_viewportLeft, m_viewportBottom), glm::vec2(m_viewportRight, m_viewportBottom));
	drawLine(glm::vec2(m_viewportRight, m_viewportBottom), glm::vec2(m_viewportRight, m_viewportTop));
	drawLine(glm::vec2(m_viewportRight, m_viewportTop), glm::vec2(m_viewportLeft, m_viewportTop));
	drawLine(glm::vec2(m_viewportLeft, m_viewportTop), glm::vec2(m_viewportLeft, m_viewportBottom));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...
		ImGui::Text("Mode Selection");
		ImGui::RadioButton("< NA >", (int*)&m_controlMode, NA);
		ImGui::RadioButton("< Rotate View >", (int*)&m_controlMode, ROTATEVIEW);
		ImGui::RadioButton("< Translate View >", (int*)&m_controlMode, TRANSLATEVIEW);
		ImGui::RadioButton("< Perspective >", (int*)&m_controlMode, PERSPECTIVE);
		ImGui::RadioButton("< Rotate Model >", (int*)&m_controlMode, ROTATEMODEL);
		ImGui::RadioButton("< Translate Model >", (int*)&m_controlMode, TRANSLATEMODEL);
		ImGui::RadioButton("< Scale Model >", (int*)&m_controlMode, SCALEMODEL);
		ImGui::RadioButton("< Viewport >", (int*)&m_controlMode, VIEWPORT);
		if(ImGui::Checkbox("Orthographic", &isOrtho)){
			m_projFlag = true;
			m_orthoFlag = true;
		}

		if( ImGui::Button( "Reset" ) ) {
			resetParameters();
		}


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		// Trigger Debug Values
		if(ImGui::Button("Show Values")) isDebug = !isDebug;
		if(isDebug){
			ImGui::Text("Cube Position: (%.2f, %.2f, %.2f)", m_cubePos.x, m_cubePos.y, m_cubePos.z);
			ImGui::Text("Cube Scale: (%.2f, %.2f, %.2f)", m_cubeScale.x, m_cubeScale.y, m_cubeScale.z);
			ImGui::Text("Cube Rotation: (%.2f, %.2f, %.2f)", m_cubeRotate.x, m_cubeRotate.y, m_cubeRotate.z);
			ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", m_camPos.x, m_camPos.y, m_camPos.z);
			ImGui::Text("Camera Rotation: (%.2f, %.2f, %.2f)", m_camRotate.x, m_camRotate.y, m_camRotate.z);
			ImGui::Text("Near: %.2f", m_near);
			ImGui::Text("Far: %.2f", m_far);
			ImGui::Text("FOV: %.2f", m_fov);
			ImGui::Text("Aspect: %.2f", m_aspect);
			ImGui::Text("Left: %.2f", m_left);
			ImGui::Text("Right: %.2f", m_right);
			ImGui::Text("Bottom: %.2f", m_bottom);
			ImGui::Text("Top: %.2f", m_top);
			ImGui::Text("Viewport Start: (%.2f, %.2f)", m_viewportStart.x, m_viewportStart.y);
			ImGui::Text("Viewport End: (%.2f, %.2f)", m_viewportEnd.x, m_viewportEnd.y);
			ImGui::Text("Viewport Left: %.2f", m_viewportLeft);
			ImGui::Text("Viewport Right: %.2f", m_viewportRight);
			ImGui::Text("Viewport Bottom: %.2f", m_viewportBottom);
			ImGui::Text("Viewport Top: %.2f", m_viewportTop);
			ImGui::Text("Control Mode: %s", controlModeNames[m_controlMode]);
			ImGui::Text("Mouse Button: %s", mouseButtonNames[m_mouseButton]);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	if(m_mouseButton != NONE){
		double deltaX = xPos - m_prevMousePos.x;
		double deltaY = yPos - m_prevMousePos.y;

		deltaX *= 0.1;
		deltaY *= 0.1;

		switch (m_controlMode)
		{
			case ROTATEVIEW:
				rotateViewHandler(deltaX, deltaY);
				break;
			case TRANSLATEVIEW:
				translateViewHandler(deltaX * 0.1, deltaY * 0.1);
				break;
			case PERSPECTIVE:
				perspectiveHandler(deltaX, deltaY);
				break;
			case ROTATEMODEL:
				rotateModelHandler(deltaX, deltaY);
				break;
			case TRANSLATEMODEL:
				translateModelHandler(deltaX * 0.1, deltaY * 0.1);
				break;
			case SCALEMODEL:
				scaleModelHandler(deltaX * 0.1, deltaY * 0.1);
				break;
			case VIEWPORT:
				m_viewportEnd.x = xPos;
				m_viewportEnd.y = yPos;
				calculateViewport();
				break;
		}

		m_prevMousePos.x = xPos;
		m_prevMousePos.y = yPos;
	}


	return eventHandled;
}

void A2::rotateViewHandler(double deltaX, double deltaY) {
	if (m_mouseButton == LEFT) {
		m_camRotate.x -= deltaX;
		if(m_camRotate.x > 180.0f){
			m_camRotate.x = -180.0f;
		}
		else if(m_camRotate.x < -180.0f){
			m_camRotate.x = 180.0f;
		}
	} else if (m_mouseButton == MIDDLE) {
		m_camRotate.y += deltaX;
	} else if (m_mouseButton == RIGHT) {
		m_camRotate.z += deltaX;
	}
	m_viewFlag = true;
}

void A2::translateViewHandler(double deltaX, double deltaY) {
	glm::vec4 delta;
	auto tf = glm::transpose(m_viewMatrix);
	if (m_mouseButton == LEFT) {
		delta = tf * glm::vec4(deltaX, 0.0f, 0.0f, 0.0f);
	} else if (m_mouseButton == MIDDLE) {
		delta = tf * glm::vec4(0.0f, deltaX, 0.0f, 0.0f);
	} else if (m_mouseButton == RIGHT) {
		delta = tf * glm::vec4(0.0f, 0.0f, deltaX, 0.0f);
	}

	m_camPos += glm::vec3(delta.x, delta.y, delta.z);
	m_viewFlag = true;
}

void A2::perspectiveHandler(double deltaX, double deltaY) {
	if (m_mouseButton == LEFT) {
		m_fov += deltaX;
		m_fov = glm::clamp(m_fov, 5.0f, 160.0f);
	} else if (m_mouseButton == MIDDLE) {
		m_near += deltaX * 0.1;
		m_near = glm::min(m_near, m_far - 0.01f); // Ensure near plane is always less than far plane
		m_near = glm::max(0.01f, m_near); // Ensure near plane is always greater than 0
	} else if (m_mouseButton == RIGHT) {
		m_far += deltaX;
		m_far = glm::max(m_near + 0.01f, m_far); // Ensure far plane is always greater than near plane
	}
	m_projFlag = true;
}

void A2::rotateModelHandler(double deltaX, double deltaY) {
	if (m_mouseButton == LEFT) {
		m_cubeRotate.x += deltaX;
	} else if (m_mouseButton == MIDDLE) {
		m_cubeRotate.y += deltaX;
	} else if (m_mouseButton == RIGHT) {
		m_cubeRotate.z += deltaX;
	}
	m_modelFlag = true;
}

void A2::translateModelHandler(double deltaX, double deltaY) {
	glm::vec4 delta;
	if (m_mouseButton == LEFT) {
		delta = m_modelMatrix * glm::vec4(deltaX, 0.0f, 0.0f, 0.0f);
	} else if (m_mouseButton == MIDDLE) {
		delta = m_modelMatrix * glm::vec4(0.0f, deltaX, 0.0f, 0.0f);
	} else if (m_mouseButton == RIGHT) {
		delta = m_modelMatrix * glm::vec4(0.0f, 0.0f, deltaX, 0.0f);
	}
	m_cubePos += glm::vec3(delta.x, delta.y, delta.z);
	m_modelFlag = true;
}

void A2::scaleModelHandler(double deltaX, double deltaY) {
	if (m_mouseButton == LEFT) {
		m_cubeScale.x += deltaX;
		m_cubeScale.x = glm::max(0.01f, m_cubeScale.x); // Ensure scale is always positive
	} else if (m_mouseButton == MIDDLE) {
		m_cubeScale.y += deltaX;
		m_cubeScale.y = glm::max(0.01f, m_cubeScale.y); // Ensure scale is always positive
	} else if (m_mouseButton == RIGHT) {
		m_cubeScale.z += deltaX;
		m_cubeScale.z = glm::max(0.01f, m_cubeScale.z); // Ensure scale is always positive
	}
	m_modelFlag = true;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		if(actions == GLFW_PRESS){
			glfwGetCursorPos(m_window, &(m_prevMousePos.x), &(m_prevMousePos.y));

			if(button == GLFW_MOUSE_BUTTON_LEFT){
				m_mouseButton = LEFT;
				glfwGetCursorPos(m_window, &(m_viewportStart.x), &(m_viewportStart.y));
				glfwGetCursorPos(m_window, &(m_viewportEnd.x), &(m_viewportEnd.y));
			}
			else if(button == GLFW_MOUSE_BUTTON_RIGHT){
				m_mouseButton = RIGHT;
			}
			else if(button == GLFW_MOUSE_BUTTON_MIDDLE){
				m_mouseButton = MIDDLE;
			}
		}

		else if(actions == GLFW_RELEASE){
			m_mouseButton = NONE;
		}
	}
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	m_windowWidth = width;
	m_windowHeight = height;
	m_projFlag = true;
	m_orthoFlag = true;

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_PRESS ) {
		switch (key) {
			case GLFW_KEY_A:
				resetParameters();
				break;
			case GLFW_KEY_Q:
				glfwSetWindowShouldClose(m_window, GL_TRUE);
				break;
			case GLFW_KEY_O:
				m_controlMode = ROTATEVIEW;
				break;
			case GLFW_KEY_E:
				m_controlMode = TRANSLATEVIEW;
				break;
			case GLFW_KEY_P:
				m_controlMode = PERSPECTIVE;
				break;
			case GLFW_KEY_J: // for testing propose
				m_controlMode = PERSPECTIVE;
				break;
			case GLFW_KEY_R:
				m_controlMode = ROTATEMODEL;
				break;
			case GLFW_KEY_T:
				m_controlMode = TRANSLATEMODEL;
				break;
			case GLFW_KEY_S:
				m_controlMode = SCALEMODEL;
				break;
			case GLFW_KEY_V:
				m_controlMode = VIEWPORT;
				break;
		}
	}

	return eventHandled;
}
