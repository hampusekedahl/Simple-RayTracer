#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

class Sphere {
public:
	Vec3f c;
	float r;
	Material m;
public:
	Sphere(const Vec3f & cen, float rad, const Material& mat) : c(cen), r(rad), m(mat) { }

	bool hit(const Ray & r, HitRec & rec) const;
	void computeSurfaceHitFields(const Ray & r, HitRec & rec) const;

};

#endif