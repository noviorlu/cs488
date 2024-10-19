// Termm-Fall 2024

#include "GeometryNode.hpp"

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
		const std::string & meshId,
		const std::string & name
)
	: SceneNode(name),
	  meshId(meshId)
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::draw(
	const glm::mat4 & modelMatrix, const glm::mat4 &viewMatrix, 
	const ShaderProgram &shader, BatchInfoMap &modelBatch) const
{
	shader.enable();
	
	//-- Set ModelView matrix:
	GLint location = shader.getUniformLocation("ModelView");
	mat4 modelView = viewMatrix * modelMatrix * trans;
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
	CHECK_GL_ERRORS;

	//-- Set NormMatrix:
	location = shader.getUniformLocation("NormalMatrix");
	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
	glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
	CHECK_GL_ERRORS;


	//-- Set Material values:
	location = shader.getUniformLocation("material.kd");
	vec3 kd = material.kd;
	glUniform3fv(location, 1, value_ptr(kd));
	CHECK_GL_ERRORS;

	BatchInfo batchInfo = modelBatch[meshId];

	//-- Now render the mesh:
	glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
	shader.disable();

	for (const SceneNode* child : children) {
		child->draw(modelMatrix * trans, viewMatrix, shader, modelBatch);
	}
}
