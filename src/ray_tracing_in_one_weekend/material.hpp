#ifndef RAYTRACER_IN_ONE_WEEKEND_MATERIAL_HPP
#define RAYTRACER_IN_ONE_WEEKEND_MATERIAL_HPP

#include <iostream>
#include "ray.hpp"
#include "hitable.hpp"
#include "rand.hpp"

vec3 random_in_unit_sphere();
vec3 reflect (const vec3& v, const vec3& normal);

class material {
public:
  virtual bool scatter(const ray& r_in, const hit_record& record, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
  // albedo is the fraction of light reflected from the material a.k.a reflectance
  vec3 albedo;

  lambertian(const vec3& a) : albedo(a) {}
  virtual bool scatter(const ray& r_in, const hit_record& record, vec3& attenuation, ray& scattered) const {
    vec3 target = record.p + record.normal + random_in_unit_sphere();
    scattered = ray(record.p, target - record.p);
    attenuation = albedo;
    return true;
  }
};

class metal : public material {
public:
  vec3 albedo;
  float fuzz;

  metal(const vec3& a, float f) : albedo(a) {
    if (f <= 1) {
      fuzz = f;
    } else {
      fuzz = 1;
      std::cout << "WARNING: Cannot use fuzz value greater than 1, defaulting to 1.\n" << std::endl;
    }
  }
  virtual bool scatter(const ray& r_in, const hit_record& record, vec3& attenuation, ray& scattered) const {
    vec3 reflected = reflect(unit_vector(r_in.direction()), record.normal);
    scattered = ray(record.p, reflected + fuzz * random_in_unit_sphere());
    attenuation = albedo;
    return (dot(scattered.direction(), record.normal) > 0);
  }
private:
};




#endif //RAYTRACER_IN_ONE_WEEKEND_MATERIAL_HPP
