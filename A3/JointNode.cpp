// Termm-Fall 2024

#include "JointNode.hpp"
#include "cs488-framework/MathUtils.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}
 //---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
}

//---------------------------------------------------------------------------------------
SceneNode* JointNode::findJointNodes(const int& nodeId, SceneNode* closestJointNode) {
	return SceneNode::findJointNodes(nodeId, this);
}

//---------------------------------------------------------------------------------------
void JointNode::rotate(char axis, float angle) {
	if(axis == 'z') return;
	glm::vec3 rot_axis;

	if(axis == 'x'){
		// clamp by min and max
		if(current_x + angle > m_joint_x.max){
			angle = m_joint_x.max - current_x;
		}
		if(current_x + angle < m_joint_x.min){
			angle = m_joint_x.min - current_x;
		}
		current_x += angle;
		rot_axis = glm::vec3(1,0,0);
	}
	if(axis == 'y'){
		// clamp by min and max
		if(current_y + angle > m_joint_y.max){
			angle = m_joint_y.max - current_y;
		}
		if(current_y + angle < m_joint_y.min){
			angle = m_joint_y.min - current_y;
		}
		current_y += angle;
		rot_axis = glm::vec3(0,1,0);
	}
	// rotate first then translate
	rot = glm::rotate(degreesToRadians(angle), rot_axis) * rot;
}

void JointNode::draw(
        const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, 
        const ShaderProgram& shader, BatchInfoMap& modelBatch) const
{
	for (const SceneNode* child : children) {
		child->draw(modelMatrix * trans * rot, viewMatrix, shader, modelBatch);
	}
}
