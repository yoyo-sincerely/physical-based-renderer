#ifndef RAYTRACER_IN_ONE_WEEKEND_SPHERE_HPP
#define RAYTRACER_IN_ONE_WEEKEND_SPHERE_HPP

#include "hitable.hpp"
#include "material.hpp"

class sphere: public hitable {
public:
  vec3 _center;
  material* _material;
  float _radius;

  sphere() {}
  sphere(vec3 center, float radius, material* mat) : _center(center), _radius(radius), _material(mat) {}
  bool hit(const ray& r, float t_min, float t_max, hit_record& record) const;
};


#endif //RAYTRACER_IN_ONE_WEEKEND_SPHERE_HPP
