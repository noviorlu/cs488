// Termm--Fall 2024

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

#define EPSILON 1e-6

bool AABB::intersect(const Ray& ray, Intersection& isect) {
float tmin = (min.x - ray.origin.x) / ray.direction.x;
float tmax = (max.x - ray.origin.x) / ray.direction.x;

if (tmin > tmax) std::swap(tmin, tmax);

float tymin = (min.y - ray.origin.y) / ray.direction.y;
float tymax = (max.y - ray.origin.y) / ray.direction.y;

if (tymin > tymax) std::swap(tymin, tymax);

if ((tmin > tymax) || (tymin > tmax)) return false;

if (tymin > tmin) tmin = tymin;
if (tymax < tmax) tmax = tymax;

float tzmin = (min.z - ray.origin.z) / ray.direction.z;
float tzmax = (max.z - ray.origin.z) / ray.direction.z;

if (tzmin > tzmax) std::swap(tzmin, tzmax);

if ((tmin > tzmax) || (tzmin > tmax)) return false;

if (tzmin > tmin) tmin = tzmin;
if (tzmax < tmax) tmax = tzmax;

return true;
}


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

#ifdef RENDER_BOUNDING_VOLUMES
    std::vector<Triangle*> triangleIndices;
    for (auto& tri : m_faces) {
        triangleIndices.push_back(&tri);
    }
    m_bvhRoot = buildBVH(triangleIndices, 0);
#endif
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


bool Mesh::triangleIntersect(Ray& ray, Intersection& isect, Triangle* tri) {
    // Get vertices of the triangle
    const glm::vec3& v0 = m_vertices[tri->v1];
    const glm::vec3& v1 = m_vertices[tri->v2];
    const glm::vec3& v2 = m_vertices[tri->v3];

    // Compute edges and the normal
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 pvec = glm::cross(ray.direction, edge2);
    float det = glm::dot(edge1, pvec);

    // Use back-face culling if needed; skip triangles facing away
    if (det < 1e-6f) return false;

    // Inverse determinant for efficient division
    float invDet = 1.0f / det;

    // Calculate u parameter and test bounds
    glm::vec3 tvec = ray.origin - v0;
    float u = glm::dot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f) return false;

    // Calculate v parameter and test bounds
    glm::vec3 qvec = glm::cross(tvec, edge1);
    float v = glm::dot(ray.direction, qvec) * invDet;
    if (v < 0.0f || u + v > 1.0f) return false;

    // Calculate t, check if within bounds, and if it's the closest intersection
    float t = glm::dot(edge2, qvec) * invDet;
    if (t < ray.maxt && t > ray.mint) {
        ray.maxt = t;
        isect.position = ray.origin + t * ray.direction;
        isect.normal = glm::normalize(glm::cross(edge1, edge2));
        return true;
    }
    return false;
}

bool Mesh::intersect(Ray& ray, Intersection& isect) {
    bool hit = false;
    
#ifndef RENDER_BOUNDING_VOLUMES
    for (Triangle& tri : m_faces) {
        hit |= triangleIntersect(ray, isect, &tri);
    }
#else
    hit = bvhIntersect(ray, isect, m_bvhRoot);
#endif
    return hit;
}

BVHNode* Mesh::buildBVH(std::vector<Triangle*>& triangleIndices, int depth) {
    BVHNode* node = new BVHNode();

    for (const auto& tri : triangleIndices) {
        AABB triBox(m_vertices[tri->v1], m_vertices[tri->v2], m_vertices[tri->v3]);
        node->bbox.expand(triBox);
    }

    if (triangleIndices.size() <= 4 || depth > 16) {
        node->triangles = triangleIndices;
        node->isLeaf = true;
        return node;
    }

    int axis = depth % 3;
    std::sort(triangleIndices.begin(), triangleIndices.end(), [&](Triangle* a, Triangle* b) {
        return m_vertices[a->v1][axis] < m_vertices[b->v1][axis];
    });

    size_t mid = triangleIndices.size() / 2;
    std::vector<Triangle*> leftTriangles(triangleIndices.begin(), triangleIndices.begin() + mid);
    std::vector<Triangle*> rightTriangles(triangleIndices.begin() + mid, triangleIndices.end());

    node->left = buildBVH(leftTriangles, depth + 1);
    node->right = buildBVH(rightTriangles, depth + 1);
    node->isLeaf = false;

    return node;
}

bool Mesh::bvhIntersect(Ray& ray, Intersection& isect, BVHNode* node) {
    if (!node->bbox.intersect(ray, isect)) {
        return false;
    }

    if (node->isLeaf) {
        bool hit = false;
        for (Triangle* tri : node->triangles) {
            if (triangleIntersect(ray, isect, tri)) {
                hit = true;
            }
        }
        return hit;
    }

    bool hitLeft = bvhIntersect(ray, isect, node->left);
    bool hitRight = bvhIntersect(ray, isect, node->right);
    return hitLeft || hitRight;
}