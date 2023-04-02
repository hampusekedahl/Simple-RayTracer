#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

class Triangle {
public:
	Vec3f vertex1;
	Vec3f vertex2;
	Vec3f vertex3;
	Vec3f normal;
	Material m;
public:
	Triangle(const Vec3f& vert1, const Vec3f& vert2, const Vec3f& vert3, const Vec3f norm, const Material mat) : vertex1(vert1), vertex2(vert2), vertex3(vert3), normal(norm), m(mat) { }

	bool hit(const Ray& r, HitRec& rec) const;
	void computeSurfaceHitFields(const Ray& r, HitRec& rec) const;

};

#endif
