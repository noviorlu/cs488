// Termm--Fall 2024

#pragma once

#include "common.hpp"
#include <glm/glm.hpp>

class Primitive {
public:
  virtual ~Primitive();
  virtual bool intersect(const Ray& ray, Intersection& isect){return false;}
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
};

class Cube : public Primitive {
public:
  virtual ~Cube();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();

  bool intersect(const Ray& ray, Intersection& isect) override;

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

  bool intersect(const Ray& ray, Intersection& isect) override;

private:
  glm::vec3 m_pos;
  double m_size;
};
