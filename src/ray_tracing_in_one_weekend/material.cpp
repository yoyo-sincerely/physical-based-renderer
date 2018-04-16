#include "material.hpp"

vec3 reflect (const vec3& v, const vec3& normal) {
  return v - 2 * dot(v, normal) * normal;
}

vec3 random_in_unit_sphere() {
  vec3 p;
  // Generate vectors in unit cube and test if they're in the unit sphere
  do {
    p = vec3(drand48(), drand48(), drand48());
  } while (p.squared_length() >= 1.0);
  return p;
}