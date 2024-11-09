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

	bool intersect(Ray& ray, Intersection& isect) override;

	Material *m_material;
	Primitive *m_primitive;
};
