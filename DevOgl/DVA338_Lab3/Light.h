#ifndef _LIGTH_H_
#define _LIGTH_H_

#include "Vec3.h"
#include "Ray.h"

class Light {
public:
	Vec3f Position;
	Vec3f Ambient;
	Vec3f Diffuse;
	Vec3f Specular;
public:
	Light(const Vec3f& pos, const Vec3f& amb, const Vec3f& diff, const Vec3f& spec) : Position(pos), Ambient(amb), Diffuse(diff), Specular(spec) { }

	void computeAmbientColor(HitRec& rec) const;
	void computeDiffuseColor(HitRec& rec) const;
	void computeSpecularColor(const Ray& r, HitRec& rec) const;
};

#endif
