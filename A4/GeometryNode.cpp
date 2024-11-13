// Termm--Fall 2024

#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the 
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and 
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

bool GeometryNode::intersect(Ray& ray, Intersection& isect){
	Ray transRay(ray);
	
	transRay.origin = glm::vec3(invtrans * glm::vec4(ray.origin, 1.0f));
	glm::vec4 transformedDirection = invtrans * glm::vec4(ray.direction, 0.0f);
	float directionScale = glm::length(glm::vec3(transformedDirection));
	transRay.direction = glm::normalize(glm::vec3(transformedDirection));
	transRay.maxt *= directionScale;
	transRay.mint *= directionScale;

	bool result = m_primitive->intersect(transRay, isect);
	if(result){
		isect.material = m_material;
		isect.position = glm::vec3(trans * glm::vec4(isect.position, 1.0f));
		
		glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(trans)));
		isect.normal = glm::normalize(normalMatrix * isect.normal);
		
		ray.maxt = transRay.maxt / directionScale;
	}
	
	return result;
}