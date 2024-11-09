// Termm--Fall 2024

#pragma once

#include "common.hpp"

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"

glm::vec3 TraceRay(
    SceneNode *root,
    const Ray &ray,
    int depth,
    const glm::vec3 &ambient,
    const std::list<Light *> &lights);

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);
