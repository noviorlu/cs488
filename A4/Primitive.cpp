// Termm--Fall 2024

#include "Primitive.hpp"
#include <algorithm>

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
}

bool NonhierSphere::intersect(const Ray& ray, Intersection& isect)
{
  glm::vec3 L = m_pos - ray.origin;
  float tca = glm::dot(L, ray.direction);
  if (tca < 0) return false;

  float d2 = glm::dot(L, L) - tca * tca;
  float r2 = m_radius * m_radius;
  if (d2 > r2) return false;

  float thc = sqrt(r2 - d2);
  float t0 = tca - thc;
  float t1 = tca + thc;

  // Ensure t0 is the smaller root
  if (t0 > t1) std::swap(t0, t1);

  // Use the farther root if the first root is negative
  if (t0 < 0) {
    t0 = t1;
    if (t0 < 0) return false; // Both intersections are behind the ray origin
  }

  // Check if this intersection is closer than any previous one
  if (t0 < isect.t) {
    // Update the intersection record
    isect.t = t0;
    isect.position = ray.origin + t0 * ray.direction;
    isect.normal = glm::normalize(isect.position - m_pos);
    return true;
  }

  return false; // The intersection is farther than the existing hit
}


bool NonhierBox::intersect(const Ray& ray, Intersection& isect)
{
  glm::vec3 p = m_pos - ray.origin;
  glm::vec3 q = p + glm::vec3(m_size, m_size, m_size);
  float tmin = -INFINITY;
  float tmax = INFINITY;

  for (int i = 0; i < 3; i++) {
    if (ray.direction[i] == 0) {
      if (p[i] > 0 || p[i] < -m_size) return false;
    } else {
      float t1 = p[i] / ray.direction[i];
      float t2 = q[i] / ray.direction[i];
      if (t1 > t2) std::swap(t1, t2);
      tmin = std::max(tmin, t1);
      tmax = std::min(tmax, t2);
      if (tmin > tmax) return false;
    }
  }

  // Check if this intersection is closer than any previous one
  if (tmin < isect.t) {
    // Update the intersection record
    isect.t = tmin;
    isect.position = ray.origin + tmin * ray.direction;

    // Determine the normal based on the intersection position
    glm::vec3 n = glm::vec3(0);
    for (int i = 0; i < 3; i++) {
      if (isect.position[i] < m_pos[i] + 0.001) n[i] = -1;
      if (isect.position[i] > m_pos[i] + m_size - 0.001) n[i] = 1;
    }
    isect.normal = n;

    return true;
  }

  return false; // The intersection is farther than the existing hit
}
