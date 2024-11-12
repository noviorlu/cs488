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

    std::string name = "Assets/" + fname;
	std::ifstream ifs( name.c_str() );
    if (!ifs.is_open()) {
        std::cerr << "Failed to open file: " << name << std::endl;
    }

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
    out << "Mesh {" << std::endl;

    // Print vertices
    out << "  Vertices:" << mesh.m_vertices.size() << std::endl;
    for (size_t idx = 0; idx < mesh.m_vertices.size(); ++idx) {
        const glm::vec3& v = mesh.m_vertices[idx];
        out << "    " << idx << ": " << v.r << "," << v.g << "," << v.b << std::endl;
    }

    // Print faces (triangles)
    out << "  Faces (Triangles):" << mesh.m_faces.size() << std::endl;
    for (size_t idx = 0; idx < mesh.m_faces.size(); ++idx) {
        const Triangle& tri = mesh.m_faces[idx];
        out << "    " << idx << ": (" << tri.v1 << ", " << tri.v2 << ", " << tri.v3 << ")" << std::endl;
    }

    out << "}" << std::endl;
    return out;
}


bool Mesh::intersect(const Ray& ray, Intersection& isect) {
    bool hit = false;
    const float EPSILON = 1e-6f;

    for (const Triangle& tri : m_faces) {
        // Get vertices of the triangle
        const glm::vec3& v0 = m_vertices[tri.v1];
        const glm::vec3& v1 = m_vertices[tri.v2];
        const glm::vec3& v2 = m_vertices[tri.v3];

        // Compute edges and the normal
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 pvec = glm::cross(ray.direction, edge2);
        float det = glm::dot(edge1, pvec);

        // Use back-face culling if needed; skip triangles facing away
        if (det < EPSILON) continue;

        // Inverse determinant for efficient division
        float invDet = 1.0f / det;

        // Calculate u parameter and test bounds
        glm::vec3 tvec = ray.origin - v0;
        float u = glm::dot(tvec, pvec) * invDet;
        if (u < 0.0f || u > 1.0f) continue;

        // Calculate v parameter and test bounds
        glm::vec3 qvec = glm::cross(tvec, edge1);
        float v = glm::dot(ray.direction, qvec) * invDet;
        if (v < 0.0f || u + v > 1.0f) continue;

        // Calculate t, check if within bounds, and if it's the closest intersection
        float t = glm::dot(edge2, qvec) * invDet;
        if (t > ray.mint && t < ray.maxt && t < isect.t) {
            isect.t = t;
            isect.position = ray.origin + t * ray.direction;
            isect.normal = glm::normalize(glm::cross(edge1, edge2));
            hit = true;
        }
    }

    return hit;
}
