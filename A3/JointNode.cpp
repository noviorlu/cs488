// Termm-Fall 2024

#include "JointNode.hpp"

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
	if(axis == 'x'){
		// clamp by min and max
		if(current_x + angle > m_joint_x.max){
			angle = m_joint_x.max - current_x;
		}
		if(current_x + angle < m_joint_x.min){
			angle = m_joint_x.min - current_x;
		}
		current_x += angle;
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
	}

	SceneNode::rotate(axis, angle);
}