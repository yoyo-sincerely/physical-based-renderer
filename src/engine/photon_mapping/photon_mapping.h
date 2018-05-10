#pragma once

//-------------------------------------------------------------------------------------------
// Includes
//-------------------------------------------------------------------------------------------
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "bitmap.h"
#include "math_util.h"
#include <list>
#include <vector>
#include <chrono>
#include "sphere.h"
#include "hitrecord.h"


#include <direct.h>
#include<iostream>
#include<string>

#define GetCurrentDir _getcwd


namespace PhotonMapping {
	void photonMapping(unsigned int * data, int width, int height, int samples);

	std::string getCurrentWorkingDir(void);

	inline double clamp(double i);

	inline unsigned int hash(const int ix, const int iy, const int iz);

	inline bool intersect(const Ray & r, double & t, int & id);

	void genp(Ray * pr, Vector3 * f, int i);

	void trace(const Ray & r, int dpt, bool m, const Vector3 & fl, const Vector3 & adj, int i);

	void trace_ray(int w, int h);

	void trace_photon(int s);

	void build_hash_grid(const int w, const int h);

	void density_estimation(Vector3 * color, int num_photon);

}

