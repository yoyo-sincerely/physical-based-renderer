#include "ray_tracing.h"

vec3 color_ray(const ray& r, hitable *world, int depth) {
  const vec3 white = vec3(1.0, 1.0, 1.0);
  const vec3 blue = vec3(0.5, 0.7, 1.0);

  hit_record record;
  // Without t_min = 0.001 (i.e. t_min = 0.0) we get shadow acne
  if (world->hit(r, 0.000001, MAXFLOAT, record)) {
    ray scattered;
    vec3 attenuation;
    if (depth < 50 && record.mat != nullptr && record.mat->scatter(r, record, attenuation, scattered)) {
      return attenuation * color_ray(scattered, world, depth + 1);
    } else {
      return vec3(0, 0, 0);
    }
  } else {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * white + t * blue;
  }
}

hitable* random_scene(int n)
{
	n = 500;
	hitable ** random_list = new hitable*[n+ 1];
	
	random_list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
}

hitable * default_scene() 
{
	hitable *list[4];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
	list[1] = new sphere(vec3(0, -100.5f, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.1));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 0.7));
}

void ray_tracing(void* buffer, int width, int height, int stride, int maxdepth, int seed){
	//std::cout << width << "\t" << height << "\t" << stride << "\t" << maxdepth << "\t" << std::endl;
  int nx = width;
  int ny = height;
  int ns = seed;

    //hitable * list = random_scene();
  hitable * list = default_scene();
  hitable *world = new hitable_list(list, 4);
  camera cam;

  for (int j = 0; j < ny; j++) {
	  auto prt = (unsigned int *)((char *)buffer + (ny - j -1) * stride);
    for (int i = 0; i < nx; i++, prt++) {
      vec3 color(0, 0, 0);
      for (int s = 0; s < ns; s++) {
        float u = float(i + drand48()) / float(nx);
        float v = float(j + drand48()) / float(ny);
        ray r = cam.get_ray(u, v);
		//std::cout << u << "    " << v << std:: endl;
		//std::cout << r.origin().x()<<"   "<< r.origin().y()<<"   "<< r.origin().z()<<"   "<< r.direction().x()<<"   "<< r.direction().y()<<"   "<< r.direction().z()<<"   "<<std::endl;
        color += color_ray(r, world, 0);
      }
      color /= float(ns);
      // Gamma correct with gamma = 2
      color = vec3(sqrt(color.r()), sqrt(color.g()), sqrt(color.b()));
	  //int ir = int(255.99*color.r());
      //int ig = int(255.99*color.g());
      //int ib = int(255.99*color.b());
	  //vec3 c = vec3(1.0, 0.0, 0.0);
	  *prt = ((255 & 255) << 24) | //alpha
				(((int)(color.b() * 255) & 255) << 16) | //blue
				(((int)(color.g() * 255) & 255) << 8) | //green
				(((int)(color.r() * 255) & 255) << 0); //red
      //std::cout << color.r()<< " " <<  color.g()<< " " <<  color.b()<< "\n";
	  //std::cout << *prt << std::endl;
    }
  }
}