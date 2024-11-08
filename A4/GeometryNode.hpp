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
		return m_primitive->intersect(ray, isect);
		// GeometryNode should always be a leaf node
	}

	Material *m_material;
	Primitive *m_primitive;
};
