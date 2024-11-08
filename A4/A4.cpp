// Termm--Fall 2024

#include <glm/ext.hpp>

#include "A4.hpp"

glm::vec3 TraceRay(
	SceneNode* root, 
	const Ray& ray, 
	int depth, 
	const glm::vec3 & ambient,
	const std::list<Light *> & lights
){
	return glm::vec3(0.0f);
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


	for (uint y = 0; y < h; ++y)
	{
		for (uint x = 0; x < w; ++x)
		{
			// Convert pixel coordinate to normalized device coordinates
			double u = (2.0 * ((x + 0.5) / double(w)) - 1.0) * half_width;
			double v = (1.0 - 2.0 * ((y + 0.5) / double(h))) * half_height;

			// Generate primary ray
			Ray primary_ray;
			primary_ray.origin = eye;
			primary_ray.direction = glm::normalize(u * u_dir + v * v_dir + w_dir);
			primary_ray.mint = 0.0f;
			primary_ray.maxt = std::numeric_limits<float>::infinity();

			// Trace the ray to determine the color at this pixel
			glm::vec3 color = TraceRay(root, primary_ray, 100, ambient, lights);

			// Set pixel color in the image
			image(x, y, 0) = color.r;
			image(x, y, 1) = color.g;
			image(x, y, 2) = color.b;
		}
	}
}
