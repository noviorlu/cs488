// Termm--Fall 2024

#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <functional>
#include <vector>
class Maze
{
public:
	Maze( size_t dim );
	~Maze();

	void reset();

	size_t getDim() const;

	int getValue( int x, int y ) const;

	void setValue( int x, int y, int h );

	void digMaze();
	void printMaze(); // for debugging

	void generateGeometry();
    std::vector<glm::vec3> getVertices();
    void NewFunction();
    std::vector<glm::uvec3> getTriangles(){ return m_triangles; }

private:
	size_t m_dim;
	int *m_values;
	void recDigMaze(int r, int c);
	int numNeighbors(int r, int c);

	// Custom hash function for glm::vec3
	struct Vec3Hash {
		std::size_t operator()(const glm::vec3& v) const {
			std::hash<float> hasher;
			std::size_t h1 = hasher(v.x);
			std::size_t h2 = hasher(v.y);
			std::size_t h3 = hasher(v.z);
			return h1 ^ (h2 << 1) ^ (h3 << 2); // Combine hash values
		}
	};

	// Custom equality function for glm::vec3
	struct Vec3Equal {
		bool operator()(const glm::vec3& v1, const glm::vec3& v2) const {
			return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
		}
	};
	std::unordered_map<glm::vec3, size_t, Vec3Hash, Vec3Equal> m_Vertices;
	std::vector<glm::uvec3> m_triangles;
};
