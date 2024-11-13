// Termm--Fall 2024

#pragma once

#include "common.hpp"
#include <glm/glm.hpp>

class Primitive {
public:
  virtual ~Primitive();
  virtual bool intersect(Ray& ray, Intersection& isect) = 0;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();

  bool intersect(Ray& ray, Intersection& isect) override;
};

class Cube : public Primitive {
public:
  virtual ~Cube();

  bool intersect(Ray& ray, Intersection& isect) override;
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();

  bool intersect(Ray& ray, Intersection& isect) override;

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();

  bool intersect(Ray& ray, Intersection& isect) override;

private:
  glm::vec3 m_pos;
  double m_size;
};
