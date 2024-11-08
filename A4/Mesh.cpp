// Termm--Fall 2024

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}

bool Mesh::intersect(const Ray& ray, Intersection& isect) {
	bool hit = false;

	// Iterate over all triangles in the mesh
	for (const Triangle& tri : m_faces) {
		const glm::vec3& v0 = m_vertices[tri.v1];
		const glm::vec3& v1 = m_vertices[tri.v2];
		const glm::vec3& v2 = m_vertices[tri.v3];

		// Calculate edges and the vector from v0 to the ray origin
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;
		glm::vec3 h = glm::cross(ray.direction, edge2);
		float a = glm::dot(edge1, h);

		// If a is near zero, the ray is parallel to the triangle
		if (std::abs(a) < 1e-6) continue;

		float f = 1.0f / a;
		glm::vec3 s = ray.origin - v0;
		float u = f * glm::dot(s, h);

		// Check if the intersection is outside the triangle
		if (u < 0.0 || u > 1.0) continue;

		glm::vec3 q = glm::cross(s, edge1);
		float v = f * glm::dot(ray.direction, q);

		// Check if the intersection is outside the triangle
		if (v < 0.0 || u + v > 1.0) continue;

		// Calculate the intersection distance along the ray
		float t = f * glm::dot(edge2, q);

		// Check if t is within the ray's bounds and if this hit is closer than the previous hit
		if (t > ray.mint && t < ray.maxt && t < isect.t) {
			// Update the intersection record with the closer hit
			isect.t = t;
			isect.position = ray.origin + t * ray.direction;
			isect.normal = glm::normalize(glm::cross(edge1, edge2)); // Triangle's face normal

			hit = true;
		}
	}

	return hit;
}

