// Termm--Fall 2024

#include <glm/ext.hpp>
#include <chrono>
#include "PhongMaterial.hpp"
#include "A4.hpp"

#define MSAA_SAMPLES 1

glm::vec3 TraceRay(
	SceneNode* root, 
	const Ray& ray, 
	int depth, 
	const glm::vec3 &ambient,
	const std::list<Light *> &lights
){
	Ray localRay(ray);
	Intersection intersection;
	if (!root->intersect(localRay, intersection)) {
		// Return background color if no intersection
		return glm::vec3(0.0f);
	}

	// [DEBUG] Normal Color
	// return 0.5f * intersection.normal + 0.5f;

	PhongMaterial *phongMaterial = static_cast<PhongMaterial *>(intersection.material);
	glm::vec3 color = ambient * phongMaterial->m_kd;

	for (const auto& light : lights) {
		glm::vec3 lightDir = glm::normalize(light->position - intersection.position);
		float lightDistance = glm::length(light->position - intersection.position) - 1e-4f;

		Ray shadowRay;
		shadowRay.origin = intersection.position + 1e-4f * lightDir;
		shadowRay.direction = lightDir;
		shadowRay.mint = 0.0f;
		shadowRay.maxt = lightDistance;

		Intersection shadowIsect;
		bool inShadow = root->intersect(shadowRay, shadowIsect);

		if (!inShadow) {
			float attenuation = 1.0f / (light->falloff[0] + light->falloff[1] * lightDistance + light->falloff[2] * lightDistance * lightDistance);

			float diff = std::max(glm::dot(intersection.normal, lightDir), 0.0f);
			color += phongMaterial->m_kd * diff * light->colour * attenuation;

			glm::vec3 viewDir = glm::normalize(ray.origin - intersection.position);
			glm::vec3 halfDir = glm::normalize(lightDir + viewDir);

			float spec = pow(std::max(glm::dot(intersection.normal, halfDir), 0.0f), phongMaterial->m_shininess);
			color += phongMaterial->m_ks * spec * light->colour * attenuation;
		}
	}

	return color;
}


void A4_Render(
	// What to render
	SceneNode *root,

	// Image to write to, set to a given width and height
	Image &image,

	// Viewing parameters
	const glm::vec3 &eye,
	const glm::vec3 &view,
	const glm::vec3 &up,
	double fovy,

	// Lighting parameters
	const glm::vec3 &ambient,
	const std::list<Light *> &lights)
{

	// Fill in raytracing code here...
	std::cout << "F24: Calling A4_Render(\n"
			  << "\t" << *root << "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
																											  "\t"
			  << "eye:  " << glm::to_string(eye) << std::endl
			  << "\t" << "view: " << glm::to_string(view) << std::endl
			  << "\t" << "up:   " << glm::to_string(up) << std::endl
			  << "\t" << "fovy: " << fovy << std::endl
			  << "\t" << "ambient: " << glm::to_string(ambient) << std::endl
			  << "\t" << "lights{" << std::endl;

	for (const Light *light : lights)
	{
		std::cout << "\t\t" << *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std::cout << ")" << std::endl;

	size_t h = image.height();
	size_t w = image.width();

	// Camera setup
	glm::vec3 w_dir = glm::normalize(view);
	glm::vec3 u_dir = glm::normalize(glm::cross(w_dir, up));
	glm::vec3 v_dir = glm::normalize(glm::cross(u_dir, w_dir));

	double aspect_ratio = double(w) / double(h);
	double fov_rad = glm::radians(fovy);
	double half_height = tan(fov_rad / 2.0);
	double half_width = aspect_ratio * half_height;


	// count renderer time
	std::cout << "Rendering..." << std::endl;
	auto start = std::chrono::high_resolution_clock::now();


	for (uint y = 0; y < h; ++y) {
		for (uint x = 0; x < w; ++x) {
			glm::vec3 color(0.0f);
			for (int i = 0; i < MSAA_SAMPLES; ++i) {
				double offset_x = (i % 2 + 0.5) / 2.0;
				double offset_y = (i / 2 + 0.5) / 2.0;

				double u = (2.0 * ((x + offset_x) / double(w)) - 1.0) * half_width;
				double v = (1.0 - 2.0 * ((y + offset_y) / double(h))) * half_height;

				Ray primary_ray;
				primary_ray.origin = eye;
				primary_ray.direction = glm::normalize(u * u_dir + v * v_dir + w_dir);
				primary_ray.mint = 0.0f;
				primary_ray.maxt = std::numeric_limits<float>::infinity();

				glm::vec3 sample_color = TraceRay(root, primary_ray, 100, ambient, lights);

				if (sample_color == glm::vec3(0.0f)) {
					sample_color = ((float)y / (float)h) * lights.front()->colour * 0.3f;
				}

				color += sample_color;
			}

			color /= float(MSAA_SAMPLES);

			image(x, y, 0) = color.r;
			image(x, y, 1) = color.g;
			image(x, y, 2) = color.b;
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Rendering time: " << elapsed.count() << "s" << std::endl;
}
