#include "material.hpp"

float schlick(float cosine, float ref_idx) {
	float f0 = (1 - ref_idx) / (1 + ref_idx);
	f0 = f0 * f0;
	return f0 + (1 + f0)*pow(1 - cosine, 5);
}

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

bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discrininant = 1.0 - ni_over_nt * ni_over_nt*(1-dt*dt);
	if (discrininant > 0)
	{
		refracted = ni_over_nt * (v - n * dt) - n * sqrt(discrininant);
		return true;
	}
	else
		return false;
}