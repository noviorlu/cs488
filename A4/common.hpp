#pragma once
#include <glm/glm.hpp>

class Material;

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
	float mint;
	float maxt;
};

struct Intersection {
	glm::vec3 position;
	glm::vec3 normal;
	float t;
	Material *material;
};