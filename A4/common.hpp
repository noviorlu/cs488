#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

// Use this #define to selectively compile your code to render the
// bounding boxes around your mesh objects. Uncomment this option
// to turn it on.
#define RENDER_BOUNDING_VOLUMES
#define MSAA_SAMPLES 1

class Material;

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
	float mint;
	float maxt;

	Ray() : origin(glm::vec3(0.0f)), direction(glm::vec3(0.0f)), mint(0.0f), maxt(std::numeric_limits<float>::infinity()) {}
	Ray(Ray const &ray) : origin(ray.origin), direction(ray.direction), mint(ray.mint), maxt(ray.maxt) {}

	friend std::ostream& operator<<(std::ostream& out, const Ray& ray){
		out << "Ray { origin: " << glm::to_string(ray.origin) << ", direction: " << glm::to_string(ray.direction) << ", mint: " << ray.mint << ", maxt: " << ray.maxt << " }";
		return out;
	}
};

struct Intersection {
	glm::vec3 position;
	glm::vec3 normal;
	Material *material;

	Intersection() : material(nullptr) {}
};