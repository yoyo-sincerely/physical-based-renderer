#pragma once
#include "../ray_tracing/Color.h"
#include "../ray_tracing/Vector.h"

#include <iostream>
#include <limits>
#include <float.h>
#include "rand.hpp"
#include "vec3.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include "hitable_list.hpp"
#include "camera.hpp"

#define MAXFLOAT LDBL_MAX

vec3 color_ray(const ray& r, hitable *world, int depth);

void ray_tracing(void * buffer, int width, int height, int stride, int maxdepth, int seed);


