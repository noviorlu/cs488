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
	mat4 modelView = viewMatrix * modelMatrix * trans;
	shader.SetUniformMat4f("ModelView", modelView);

	//-- Set NormMatrix:
	mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
	shader.SetUniformMat3f("NormalMatrix", normalMatrix);

	//-- Set Material values:
	vec3 kd = material.kd;
	shader.SetUniform3fv("material.kd", kd);
 
	//-- Set SceneNode ID:
	shader.SetUniform1i("nodeId", m_nodeId);

	BatchInfo batchInfo = modelBatch[meshId];

	//-- Now render the mesh:
	glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
	shader.disable();
	
	for (const SceneNode* child : children) {
		child->draw(modelMatrix * trans, viewMatrix, shader, modelBatch);
	}
}
