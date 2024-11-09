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

	std::cout << *this << std::endl;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
    out << "Mesh {" << std::endl;

    // Print vertices
    out << "  Vertices:" << std::endl;
    for (size_t idx = 0; idx < mesh.m_vertices.size(); ++idx) {
        const glm::vec3& v = mesh.m_vertices[idx];
        out << "    " << idx << ": " << glm::to_string(v) << std::endl;
    }

    // Print faces (triangles)
    out << "  Faces (Triangles):" << std::endl;
    for (size_t idx = 0; idx < mesh.m_faces.size(); ++idx) {
        const Triangle& tri = mesh.m_faces[idx];
        out << "    " << idx << ": (" << tri.v1 << ", " << tri.v2 << ", " << tri.v3 << ")" << std::endl;
    }

    out << "}" << std::endl;
    return out;
}


bool Mesh::intersect(const Ray& ray, Intersection& isect) {
    bool hit = false;

    for (const Triangle& tri : m_faces) {
        const glm::vec3& v0 = m_vertices[tri.v1];
        const glm::vec3& v1 = m_vertices[tri.v2];
        const glm::vec3& v2 = m_vertices[tri.v3];

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 h = glm::cross(ray.direction, edge2);
        float a = glm::dot(edge1, h);

        if (std::abs(a) < 1e-6) continue;

        float f = 1.0f / a;
        glm::vec3 s = ray.origin - v0;
        float u = f * glm::dot(s, h);

        if (u < 0.0f || u > 1.0f) continue;

        glm::vec3 q = glm::cross(s, edge1);
        float v = f * glm::dot(ray.direction, q);

        if (v < 0.0f || u + v > 1.0f) continue;

        float t = f * glm::dot(edge2, q);

        if (t > ray.mint && t < ray.maxt && t < isect.t) {
            isect.t = t;
            isect.position = ray.origin + t * ray.direction;
            isect.normal = glm::normalize(glm::cross(edge1, edge2));

            hit = true;
        }
    }

    return hit;
}
