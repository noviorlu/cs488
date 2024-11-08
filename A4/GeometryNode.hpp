// Termm--Fall 2024

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim, 
		Material *mat = nullptr );

	void setMaterial( Material *material );

	bool intersect(const Ray& ray, Intersection& isect) override{
		if(m_primitive->intersect(ray, isect)){
			isect.material = m_material;
			return true;
		}
		return false;
	}

	Material *m_material;
	Primitive *m_primitive;
};
