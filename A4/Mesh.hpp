// Termm--Fall 2024

#pragma once

#include <vector>
#include <iosfwd>
#include <string>
#include <algorithm>

#include <glm/glm.hpp>

#include "common.hpp"
#include "Primitive.hpp"

class AABB {
public:
	AABB() : min(std::numeric_limits<float>::max()), max(std::numeric_limits<float>::lowest()) {}

	AABB(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
		this->expand(v1);
		this->expand(v2);
		this->expand(v3);
	}

	void expand(const glm::vec3& point) {
		min = glm::min(min, point);
		max = glm::max(max, point);
	}

	void expand(const AABB& bbox) {
		min = glm::min(min, bbox.min);
		max = glm::max(max, bbox.max);
	}

	// do not update ray since it is not intersecting with actual geometry
	bool intersect(const Ray& ray, Intersection& isect);

private:
	glm::vec3 min;
	glm::vec3 max;
};

struct Triangle
{
	size_t v1;
	size_t v2;
	size_t v3;

	Triangle( size_t pv1, size_t pv2, size_t pv3 )
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
	{}
};

class BVHNode {
public:
	BVHNode() : left(nullptr), right(nullptr) {}

	AABB bbox;
	BVHNode* left;
	BVHNode* right;
	bool isLeaf = false;
	std::vector<Triangle*> triangles;
};


// A polygonal mesh.
class Mesh : public Primitive {
public:
  	Mesh( const std::string& fname );
	bool intersect(Ray& ray, Intersection& isect) override;
	bool bvhIntersect(Ray& ray, Intersection& isect, BVHNode* node);
	bool triangleIntersect(Ray& ray, Intersection& isect, Triangle* tri);


	BVHNode* buildBVH(std::vector<Triangle*>& triangleIndices, int depth = 0);
private:
	std::vector<glm::vec3> m_vertices;
	std::vector<Triangle> m_faces;

	BVHNode* m_bvhRoot;

    friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};
